#include "hands.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <ext/alloc_traits.h>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "analysis.h"
#include "gamestate.h"
#include "hand.h"
#include "handnode.h"
#include "meld.h"
#include "statefunctions.h"
#include "stateutilities.h"
#include "walls.h"
#include "winds.h"

namespace Mahjong {

auto countPiece(const GameState& state, int player, Piece p) -> int {
  int count = 0;
  for (const auto& piece : state.hands.at(player).live) {
    if (piece == p) {
      count++;
    }
  }
  return count;
}

const std::vector<Mahjong::yakuFunc> yakuFunctions = {
  isFullyConcealedHand, isPinfu, isPureDoubleChi, isAllSimples,
  isMixedTripleChi, isPureStraight, isWindOrDragonPon, isOutsideHand,
  isAfterAKan, isRobbingAKan, isBottomOfTheSea, isSevenPairs,
  isTriplePon, isThreeConcealedPons, isThreeKans,
  isAllPons, isHalfFlush, isLittleThreeDragons, isAllTerminalsAndHonors,
  isTerminalsInAllSets, isTwicePureDoubleChi, isBlessingOfMan, isFullFlush};

const std::vector<yakuFunc> yakumanFunctions = {
  isThirteenOrphans, isNineGates, isBlessingOfHeaven,
  isBlessingOfEarth, isFourConcealedPon, isFourKans,
  isAllGreen, isAllTerminals, isAllHonors, isBigThreeDragons,
  isLittleFourWinds, isBigFourWinds, isMaxBranches};

auto scoreHand(const GameState& state, int player) -> Score {
  auto root = breakdownHand(state.hands.at(player).live);
  Score s;
  s.han = 0;
  s.yakuman = 0;
  s.fu = 0;
  if (!root->IsComplete() &&
      (isThirteenOrphans(state, player) == 0) &&
      (isSevenPairs(state, player) == 0)) {
    return s;
  }
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    Score branchscore;
    for (const auto& node : branch) {
      if (node->type == Node::Single) {
        continue;
      }
    }

    for (auto yakuFunction : yakuFunctions) {
      branchscore.han += yakuFunction(state, player, branch);
    }
    for (auto yakuFunction : yakumanFunctions) {
      branchscore.yakuman += yakuFunction(state, player, branch);
    }
    for (const auto& dora : state.walls.GetDoras()) {
      for (const auto& p : state.hands.at(player).live) {
        if (p == dora) {
          branchscore.han++;
        }
      }
      for (const auto& meld : state.hands.at(player).melds) {
        if (meld.start == dora) {
          branchscore.han++;
        }
        if (meld.type == Meld::Chi) {
          if (meld.start + 1 == dora) {
            branchscore.han++;
          }
          if (meld.start + 2 == dora) {
            branchscore.han++;
          }
        }
      }
    }
    branchscore.fu = getFu(state, player, branch);
    if (getBasicPoints(branchscore) > getBasicPoints(s)) {
      s = branchscore;
    }
  }

  return s;
}

const int LOW_HAN_BASIC_POINTS = 2000;
const int MANGAN_BASIC_POINTS = 2000;
const int MANGAN_MIN_HAN = 5;
const int HANEMAN_BASIC_POINTS = 3000;
const int HANEMAN_MIN_HAN = 6;
const int BAIMAN_BASIC_POINTS = 4000;
const int BAIMAN_MIN_HAN = 8;
const int SANBAIMAN_BASIC_POINTS = 6000;
const int SANBAIMAN_MIN_HAN = 11;
const int YAKUMAN_BASIC_POINTS = 8000;

auto getBasicPoints(Score s) -> int {
  if (s.yakuman > 0) {
    return s.yakuman * YAKUMAN_BASIC_POINTS;
  }
  if (s.han >= MANGAN_MIN_HAN) {
    if (s.han >= SANBAIMAN_MIN_HAN) {
      return SANBAIMAN_BASIC_POINTS;
    }
    if (s.han >= BAIMAN_MIN_HAN) {
      return BAIMAN_BASIC_POINTS;
    }
    if (s.han >= HANEMAN_MIN_HAN) {
      return HANEMAN_BASIC_POINTS;
    }
    if (s.han >= SANBAIMAN_MIN_HAN) {
      return SANBAIMAN_BASIC_POINTS;
    }
    if (s.han >= MANGAN_MIN_HAN) {
      return MANGAN_BASIC_POINTS;
    }
  }
  int p = s.fu * (2 << (1 + s.han));
  if (p > LOW_HAN_BASIC_POINTS) {
    return LOW_HAN_BASIC_POINTS;
  }
  return p;
}

const int SEVENPAIRS = 25;
const int PINFU_DISCARD = 30;
const int PINFU_SELFDRAW = 20;
const int CONCEALED_DISCARD = 25;
const int OPEN_OR_SELFDRAW = 20;

const int SIMPLEPON = 2;
const int TERM_HONORPON = 4;
const int SIMPLEKAN = 8;
const int TERM_HONORKAN = 16;

const int CONCEALED_MUL = 2;
const int CSIMPLEPON = SIMPLEPON * CONCEALED_MUL;
const int CTERM_HONORPON = TERM_HONORPON * CONCEALED_MUL;
const int CSIMPLEKAN = SIMPLEKAN * CONCEALED_MUL;
const int CTERM_HONORKAN = TERM_HONORKAN * CONCEALED_MUL;

const int DRAGONSEATPREVALENTWIND = 2;
const int EDGECLOSEDPAIRWAIT = 2;
const int SELFDRAW = 2;
const int OPENPINFU = 2;

const int LOWEDGEWAIT = 3;
const int HIGHEDGEWAIT = 7;

const int FU_ROUNDING = 10;

auto getFu(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  if (isSevenPairs(state, player, branch) != 0) {
    return SEVENPAIRS;
  }
  if (isPinfu(state, player, branch) != 0) {
    if (state.hasRonned.at(player)) {
      return PINFU_DISCARD;
    }
    return PINFU_SELFDRAW;
  }
  int fu = 0;
  if (!state.hands.at(player).open && state.hasRonned.at(player)) {
    fu = CONCEALED_DISCARD;
  } else {
    fu = OPEN_OR_SELFDRAW;
  }
  bool open = state.hands.at(player).open;
  if (isOpenPinfu(state, player, branch)) {
    fu += OPENPINFU;
  } else if (!state.hasRonned.at(player)) {
    fu += SELFDRAW;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::Kan) {
      if (!meld.start.isHonor() && !meld.start.isTerminal()) {
        fu += open ? SIMPLEKAN : CSIMPLEKAN;
      } else {
        fu += open ? TERM_HONORKAN : CTERM_HONORKAN;
      }
    }
  }
  for (const auto& node : branch) {
    if (node->type == Node::PonSet) {
      if (!node->start.isHonor() && !node->start.isTerminal()) {
        fu += open ? SIMPLEPON : CSIMPLEPON;
      } else {
        fu += open ? TERM_HONORPON : CTERM_HONORPON;
      }
    }
    if (open) {
      continue;
    }
    if (node->type == Node::Pair) {
      if (node->start.isHonor()) {
        if (node->start == Piece::GREEN_DRAGON ||
            node->start == Piece::RED_DRAGON ||
            node->start == Piece::WHITE_DRAGON ||
            node->start == (state.roundNum > 3 ? Piece::SOUTH_WIND : Piece::EAST_WIND)) {
          fu += DRAGONSEATPREVALENTWIND;
        }
      }
      if (node->start == state.pendingPiece) {
        fu += EDGECLOSEDPAIRWAIT;
      }
    }
    if (node->type == Node::ChiSet) {
      if (node->start + 1 == state.pendingPiece) {
        fu += EDGECLOSEDPAIRWAIT;
      }
      if (node->start.getPieceNum() == 1 && state.pendingPiece.getPieceNum() == LOWEDGEWAIT &&
          node->start.getSuit() == state.pendingPiece.getSuit()) {
        fu += EDGECLOSEDPAIRWAIT;
      }
      if (node->start.getPieceNum() == HIGHEDGEWAIT && state.pendingPiece == node->start) {
        fu += EDGECLOSEDPAIRWAIT;
      }
    }
  }
  if ((fu % FU_ROUNDING) != 0) {
    return fu;
  }
  return fu + (FU_ROUNDING - (fu % FU_ROUNDING));  //rounding up to multiple of ten
}

auto isOpenPinfu(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> bool {
  for (const auto& node : branch) {
    if (node->type == Node::PonSet) {
      return false;
    }
    if (node->type == Node::Pair) {
      if (node->start == Piece::RED_DRAGON ||
          node->start == Piece::WHITE_DRAGON ||
          node->start == Piece::GREEN_DRAGON) {
        return false;
      }
      if (node->start == Piece::SOUTH_WIND && state.roundNum > 3) {
        return false;
      }
      if (node->start == Piece::EAST_WIND && state.roundNum < 4) {
        return false;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type > Meld::Chi) {
      return false;
    }
  }
  auto waits = isInTenpai(state.hands.at(player).live, /*allWaits=*/true);
  return waits.size() != 1;
}

auto isComplete(const GameState& state, int player) -> bool {
  auto root = breakdownHand(state.hands.at(player).live);
  if (!root->IsComplete() &&
      (isThirteenOrphans(state, player) == 0) &&
      (isSevenPairs(state, player) == 0)) {
    return false;
  }
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    for (const auto& node : branch) {
      if (node->type == Node::Single) {
        continue;
      }
    }

    for (auto yakuFunction : yakuFunctions) {
      if (yakuFunction(state, player, branch) > 0) {
        return true;
      }
    }
    for (auto yakuFunction : yakumanFunctions) {
      if (yakuFunction(state, player, branch) > 0) {
        return true;
      }
    }
  }

  return false;
}

const std::vector<Piece> PIECE_SET = {
  Piece::ONE_BAMBOO, Piece::TWO_BAMBOO, Piece::THREE_BAMBOO, Piece::FOUR_BAMBOO,
  Piece::FIVE_BAMBOO, Piece::SIX_BAMBOO, Piece::SEVEN_BAMBOO, Piece::EIGHT_BAMBOO,
  Piece::NINE_BAMBOO, Piece::ONE_PIN, Piece::TWO_PIN, Piece::THREE_PIN, Piece::FOUR_PIN,
  Piece::FIVE_PIN, Piece::SIX_PIN, Piece::SEVEN_PIN, Piece::EIGHT_PIN, Piece::NINE_PIN,
  Piece::ONE_CHARACTER, Piece::NINE_CHARACTER, Piece::TWO_CHARACTER, Piece::Piece::THREE_CHARACTER, Piece::FOUR_CHARACTER, Piece::FIVE_CHARACTER,
  Piece::SIX_CHARACTER, Piece::SEVEN_CHARACTER, Piece::EIGHT_CHARACTER, Piece::Piece::WHITE_DRAGON, Piece::GREEN_DRAGON, Piece::RED_DRAGON, Piece::EAST_WIND,
  Piece::SOUTH_WIND, Piece::NORTH_WIND, Piece::WEST_WIND};

const int MAX_SINGLES = 15;
auto countSingles(std::vector<Piece> hand) -> int {
  auto root = breakdownHand(std::move(hand));
  int minSingles = MAX_SINGLES;
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    int singles = 0;
    for (const auto& node : branch) {
      if (node->type == Node::Single) {
        singles++;
      }
    }
    if (singles < minSingles) {
      minSingles = singles;
    }
  }
  return minSingles;
}

//this is an extrememly inefficient algorithm but it's probably good enough for
//the frequency it needs to be ran
//will revisit if necessary
//assumption is 14 piece hand
auto isInTenpai13Pieces(std::vector<Piece> hand, bool allWaits) -> std::vector<Piece> {
  int minSingles = countSingles(hand);
  //These numbers were found by looking at a lot of handtrees and their single count
  //NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  if (minSingles > 5 || minSingles == 3 || minSingles == 0) {
    return {};
  }
  std::array<int8_t, Piece::PIECESIZE> counts = {};
  std::vector<Piece> waits;
  for (const auto& p : hand) {
    counts.at(p.toUint8_t())++;
  }
  for (const auto& p : PIECE_SET) {
    if (counts.at(p.toUint8_t()) == 4) {
      continue;
    }
    hand.push_back(p);
    auto root = breakdownHand(hand);
    if (root->IsComplete()) {
      waits.push_back(p);
      if (!allWaits) {
        return waits;
      }
    }

    hand.pop_back();
  }
  return waits;
}

const int PIECESINAHAND = 14;

auto isInTenpai(std::vector<Piece> hand, bool allWaits) -> std::vector<Piece> {
  if (hand.empty()) {
    return {};
  }
  int minSingles = countSingles(hand);
  //These numbers are the same as above except one more piece means 1 higher on the single count
  //NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  if (minSingles > 6 || minSingles == 1 || minSingles == 4 || minSingles == 0) {
    return {};
  }
  std::array<bool, Piece::PIECESIZE> removedbefore = {};
  std::vector<Piece> waits;
  for (int i = 0; i < PIECESINAHAND; i++) {
    Piece removed = hand.front();
    hand.erase(hand.begin());
    if (removedbefore.at(removed.toUint8_t())) {
      hand.push_back(removed);
      continue;
    }
    removedbefore.at(removed.toUint8_t()) = true;
    std::vector<Piece> tempwaits = isInTenpai13Pieces(hand, allWaits);
    if (!tempwaits.empty()) {
      if (!allWaits) {
        return tempwaits;
      }
      waits.insert(waits.begin(), tempwaits.begin(), tempwaits.end());
    }
    hand.push_back(removed);
  }
  return waits;
}

//see above comment
auto getRiichiDiscard(std::vector<Piece> hand) -> std::vector<Piece> {
  if (hand.empty()) {
    return {};
  }
  std::array<int8_t, Piece::PIECESIZE> counts = {};
  std::array<bool, Piece::PIECESIZE> removedbefore = {};
  std::vector<Piece> removeMe;
  for (const auto& p : hand) {
    counts.at(p.toUint8_t())++;
  }
  for (int i = 0; i < PIECESINAHAND; i++) {
    Piece removed = hand.front();
    hand.erase(hand.begin());
    if (removedbefore.at(removed.toUint8_t())) {
      hand.push_back(removed);
      continue;
    }
    removedbefore.at(removed.toUint8_t()) = true;
    for (const auto& p : PIECE_SET) {
      if (counts.at(p.toUint8_t()) == 4 || p == removed) {
        continue;
      }
      hand.push_back(p);
      auto root = breakdownHand(hand);
      if (root->IsComplete()) {
        removeMe.push_back(removed);
      }

      hand.pop_back();
    }
    hand.push_back(removed);
  }
  return removeMe;
}

const int ONEROUNDOFTURNS = 4;
auto isRiichi(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  if (state.hands.at(player).riichi) {
    int han = 1;
    if (state.turnNum < 4 && state.lastCall < 0) {
      han++;
    }
    if (state.turnNum - state.hands.at(player).riichiRound <= ONEROUNDOFTURNS && state.lastCall < state.hands.at(player).riichiRound) {
      han++;
    }
  }
  return 0;
}

auto isFullyConcealedHand(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  if (state.currentPlayer == player && !state.hands.at(player).open && state.walls.GetRemainingPieces() > 0) {
    return 1;
  }
  return 0;
}

auto isPinfu(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  if (state.hands.at(player).open) {
    return 0;
  }
  for (const auto& node : branch) {
    if (node->type == Node::PonSet) {
      return 0;
    }
    if (node->type == Node::Pair) {
      if (node->start == Piece::RED_DRAGON ||
          node->start == Piece::WHITE_DRAGON ||
          node->start == Piece::GREEN_DRAGON) {
        return 0;
      }
      if (node->start == Piece::SOUTH_WIND && state.roundNum > 3) {
        return 0;
      }
      if (node->start == Piece::EAST_WIND && state.roundNum < 4) {
        return 0;
      }
    }
  }
  auto waits = isInTenpai(state.hands.at(player).live, /*allWaits=*/true);
  if (waits.size() == 1) {
    return 0;
  }
  return 1;
}

auto isPureDoubleChi(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  if (state.hands.at(player).open) {
    return 0;
  }
  if (isTwicePureDoubleChi(state, player, branch) != 0) {
    return 0;
  }
  for (size_t i = 0; i < branch.size(); i++) {
    if (branch.at(i)->type != Node::ChiSet) {
      continue;
    }
    for (size_t j = 0; j < branch.size(); j++) {
      if (i == j) {
        continue;
      }
      if (branch.at(i)->type == branch[j]->type && branch.at(i)->start == branch[j]->start) {
        return 1;
      }
    }
  }
  return 0;
}

auto isAllSimples(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.isHonor() || piece.isTerminal()) {
      return 0;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.isHonor() || meld.start.isTerminal()) {
      return 0;
    }
  }
  return 1;
}

const int PIECESINASUIT = 9;
auto isMixedTripleChi(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  std::array<bool, PIECESINASUIT> bambooChi = {};
  std::array<bool, PIECESINASUIT> charChi = {};
  std::array<bool, PIECESINASUIT> pinChi = {};
  for (const auto& node : branch) {
    if (node->type == Node::ChiSet) {
      if (node->start.getSuit() == Piece::BAMBOO_SUIT) {
        bambooChi.at(node->start.getPieceNum()) = true;
      }
      if (node->start.getSuit() == Piece::CHARACTER_SUIT) {
        charChi.at(node->start.getPieceNum()) = true;
      }
      if (node->start.getSuit() == Piece::PIN_SUIT) {
        pinChi.at(node->start.getPieceNum()) = true;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::Chi) {
      if (meld.start.getSuit() == Piece::BAMBOO_SUIT) {
        bambooChi.at(meld.start.getPieceNum()) = true;
      }
      if (meld.start.getSuit() == Piece::CHARACTER_SUIT) {
        charChi.at(meld.start.getPieceNum()) = true;
      }
      if (meld.start.getSuit() == Piece::PIN_SUIT) {
        pinChi.at(meld.start.getPieceNum()) = true;
      }
    }
  }
  for (int i = 0; i < PIECESINASUIT; i++) {
    if (bambooChi.at(i) && charChi.at(i) && pinChi.at(i)) {
      return state.hands.at(player).open ? 1 : 2;
    }
  }
  return 0;
}

const int FIRSTCHISTART = 1;
const int SECONDCHISTART = 4;
const int THIRDCHISTART = 7;
auto isPureStraight(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  std::array<bool, 3> bambooChi = {};
  std::array<bool, 3> charChi = {};
  std::array<bool, 3> pinChi = {};
  for (const auto& node : branch) {
    if (node->type == Node::ChiSet) {
      int ind = 0;
      if (node->start.getPieceNum() == FIRSTCHISTART) {
        ind = 0;
      } else if (node->start.getPieceNum() == SECONDCHISTART) {
        ind = 1;
      } else if (node->start.getPieceNum() == THIRDCHISTART) {
        ind = 2;
      } else {
        continue;
      }
      if (node->start.getSuit() == Piece::BAMBOO_SUIT) {
        bambooChi.at(ind) = true;
      }
      if (node->start.getSuit() == Piece::CHARACTER_SUIT) {
        charChi.at(ind) = true;
      }
      if (node->start.getSuit() == Piece::PIN_SUIT) {
        pinChi.at(ind) = true;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::Chi) {
      int ind = 0;
      if (meld.start.getPieceNum() == FIRSTCHISTART) {
        ind = 0;
      } else if (meld.start.getPieceNum() == SECONDCHISTART) {
        ind = 1;
      } else if (meld.start.getPieceNum() == THIRDCHISTART) {
        ind = 2;
      } else {
        continue;
      }
      if (meld.start.getSuit() == Piece::BAMBOO_SUIT) {
        bambooChi.at(ind) = true;
      }
      if (meld.start.getSuit() == Piece::CHARACTER_SUIT) {
        charChi.at(ind) = true;
      }
      if (meld.start.getSuit() == Piece::PIN_SUIT) {
        pinChi.at(ind) = true;
      }
    }
  }
  int han = state.hands.at(player).open ? 1 : 2;
  if (bambooChi[0] && bambooChi[1] && bambooChi[2]) {
    return han;
  }
  if (charChi[0] && charChi[1] && charChi[2]) {
    return han;
  }
  if (pinChi[0] && pinChi[1] && pinChi[2]) {
    return han;
  }
  return 0;
}

auto isWindOrDragonPon(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  std::vector<Piece> matches{
    Piece::WHITE_DRAGON, Piece::GREEN_DRAGON, Piece::RED_DRAGON,
    Piece::fromWind(GetSeat(state.roundNum, player)),
    Piece::fromWind(state.roundNum > 3 ? South : East)};
  int han = 0;
  for (const auto& match : matches) {
    bool isMatch = false;
    for (const auto& node : branch) {
      if (node->type == Node::PonSet && node->start == match) {
        isMatch = true;
        break;
      }
    }
    for (const auto& meld : state.hands.at(player).melds) {
      if (meld.type >= Meld::Pon && meld.start == match) {
        isMatch = true;
        break;
      }
    }
    if (isMatch) {
      han++;
    }
  }
  return han;
}

auto isOutsideHand(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  if ((isTerminalsInAllSets(state, player, branch) != 0) || (isAllTerminalsAndHonors(state, player, branch) != 0)) {
    return 0;
  }
  bool chi = false;
  for (const auto& node : branch) {
    if (node->type == Node::ChiSet) {
      if (node->start.isTerminal() ||
          (node->start + 2).isTerminal()) {
        chi = true;
      } else {
        return 0;
      }
    } else {
      if (!node->start.isTerminal() &&
          !node->start.isHonor()) {
        return 0;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::Chi) {
      if (meld.start.isTerminal() ||
          (meld.start + 2).isTerminal()) {
        chi = true;
      } else {
        return 0;
      }
    } else {
      if (!meld.start.isTerminal() &&
          !meld.start.isHonor()) {
        return 0;
      }
    }
  }
  if (!chi) {
    return 0;
  }
  return state.hands.at(state.currentPlayer).open ? 1 : 2;
}

auto isAfterAKan(const GameState& state, int /*unused*/, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  if (state.nextState == KanDiscard) {
    return 1;
  }
  return 0;
}

auto isRobbingAKan(const GameState& state, int /*unused*/, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  if (state.nextState == KanDiscard) {
    return 1;
  }
  return 0;
}

auto isBottomOfTheSea(const GameState& state, int /*unused*/, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  if (state.walls.GetRemainingPieces() == 0) {
    return 1;
  }
  return 0;
}

auto isSevenPairs(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  if (state.hands.at(player).open) {
    return 0;
  }
  for (size_t i = 0; i < state.hands.at(player).live.size(); i += 2) {
    if (state.hands.at(player).live.at(i) != state.hands.at(player).live[i + 1]) {
      return 0;
    }
  }
  return 2;
}

auto isTriplePon(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  std::array<bool, PIECESINASUIT> bambooPon = {};
  std::array<bool, PIECESINASUIT> charPon = {};
  std::array<bool, PIECESINASUIT> pinPon = {};
  for (const auto& node : branch) {
    if (node->type == Node::PonSet) {
      if (node->start.getSuit() == Piece::BAMBOO_SUIT) {
        bambooPon.at(node->start.getPieceNum() - 1) = true;
      }
      if (node->start.getSuit() == Piece::CHARACTER_SUIT) {
        charPon.at(node->start.getPieceNum() - 1) = true;
      }
      if (node->start.getSuit() == Piece::PIN_SUIT) {
        pinPon.at(node->start.getPieceNum() - 1) = true;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::Pon) {
      if (meld.start.getSuit() == Piece::BAMBOO_SUIT) {
        bambooPon.at(meld.start.getPieceNum() - 1) = true;
      }
      if (meld.start.getSuit() == Piece::CHARACTER_SUIT) {
        charPon.at(meld.start.getPieceNum() - 1) = true;
      }
      if (meld.start.getSuit() == Piece::PIN_SUIT) {
        pinPon.at(meld.start.getPieceNum() - 1) = true;
      }
    }
  }
  for (int i = 0; i < PIECESINASUIT; i++) {
    if (bambooPon.at(i) && charPon.at(i) && pinPon.at(i)) {
      return 2;
    }
  }
  return 0;
}

auto isThreeConcealedPons(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  int concealedPons = 0;
  for (const auto& node : branch) {
    if (node->type == Node::PonSet) {
      concealedPons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::ConcealedKan) {
      concealedPons++;
    }
  }
  if (concealedPons >= 3) {
    return 2;
  }
  return 0;
}

auto isThreeKans(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  int kans = 0;
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::Kan) {
      kans++;
    }
  }
  if (kans >= 3) {
    return 2;
  }
  return 0;
}

auto isAllPons(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  if (isAllTerminalsAndHonors(state, player, branch) != 0) {
    return 0;
  }
  for (const auto& node : branch) {
    if (node->type != Node::PonSet) {
      return 0;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::Chi) {
      return 0;
    }
  }
  return 2;
}

auto isHalfFlush(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  if (isFullFlush(state, player, branch) != 0) {
    return 0;
  }
  int suit = state.hands.at(player).live.front().getSuit();
  bool honors = false;
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.isHonor()) {
      honors = true;
      continue;
    }
    if (suit != piece.getSuit()) {
      return 0;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.isHonor()) {
      honors = true;
      continue;
    }
    if (suit != meld.start.getSuit()) {
      return 0;
    }
  }
  if (honors) {
    return state.hands.at(player).open ? 2 : 3;
  }
  return 0;
}

auto isLittleThreeDragons(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  bool pair = false;
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start.toUint8_t()) {
      case Piece::RED_DRAGON:
      case Piece::GREEN_DRAGON:
      case Piece::WHITE_DRAGON:
        break;
      default:
        continue;
    }
    if (node->type != Node::Pair) {
      pons++;
    } else {
      if (pair) {
        return 0;
      }
      pair = true;
    }
  }
  if (!pair) {
    return 0;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    switch (meld.start.toUint8_t()) {
      case Piece::RED_DRAGON:
      case Piece::GREEN_DRAGON:
      case Piece::WHITE_DRAGON:
        break;
      default:
        continue;
    }
    pons++;
  }
  if (pons == 2 && pair) {
    return 2;
  }
  return 0;
}

auto isAllTerminalsAndHonors(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isHonor() && !piece.isTerminal()) {
      return 0;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (!meld.start.isHonor() && !meld.start.isTerminal()) {
      return 0;
    }
  }
  return 2;
}

auto isTerminalsInAllSets(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  if (isFullFlush(state, player, branch) != 0) {
    return 0;
  }
  bool chi = false;
  for (const auto& node : branch) {
    if (node->type == Node::ChiSet) {
      if (node->start.isTerminal() ||
          (node->start + 2).isTerminal()) {
        chi = true;
      } else {
        return 0;
      }
    } else {
      if (!node->start.isTerminal()) {
        return 0;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::Chi) {
      if (meld.start.isTerminal() ||
          (meld.start + 2).isTerminal()) {
        chi = true;
      } else {
        return 0;
      }
    } else {
      if (!meld.start.isTerminal()) {
        return 0;
      }
    }
  }
  if (!chi) {
    return 0;
  }
  return state.hands.at(state.currentPlayer).open ? 2 : 3;
}

auto isTwicePureDoubleChi(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  if (state.hands.at(player).open) {
    return 0;
  }
  int twice = 0;
  for (size_t i = 0; i < branch.size(); i++) {
    if (branch.at(i)->type != Node::ChiSet) {
      continue;
    }
    for (size_t j = 0; j < branch.size(); j++) {
      if (i == j) {
        continue;
      }
      if (branch.at(i)->type == branch[j]->type && branch.at(i)->start == branch[j]->start) {
        twice++;
      }
    }
  }
  if (twice == 2) {
    return 3;
  }
  return 0;
}

auto isBlessingOfMan(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  if (state.hands.at(player).open) {
    return 0;
  }
  if (state.turnNum > 3) {
    return 0;
  }
  if (state.lastCall >= 0) {
    return 0;
  }
  if (state.hasRonned.at(player)) {
    //NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    return 5;
  }
  return 0;
}

auto isFullFlush(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  int suit = state.hands.at(player).live.front().getSuit();
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.getSuit() != suit) {
      return 0;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.getSuit() != suit) {
      return 0;
    }
  }
  //NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  return state.hands.at(player).open ? 5 : 6;
}

auto isThirteenOrphans(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  if (state.hands.at(player).open) {
    return 0;
  }
  std::map<Piece, bool> pieces = {
    {Piece::ONE_CHARACTER, false}, {Piece::NINE_CHARACTER, false}, {Piece::ONE_PIN, false}, {Piece::NINE_PIN, false}, {Piece::ONE_BAMBOO, false}, {Piece::NINE_BAMBOO, false}, {Piece::EAST_WIND, false}, {Piece::SOUTH_WIND, false}, {Piece::WEST_WIND, false}, {Piece::NORTH_WIND, false}, {Piece::RED_DRAGON, false}, {Piece::WHITE_DRAGON, false}, {Piece::GREEN_DRAGON, false}};
  bool duplicate = false;
  for (const auto& piece : state.hands.at(player).live) {
    if (pieces.contains(piece)) {
      if (pieces[piece]) {
        duplicate = true;
      } else {
        pieces[piece] = true;
      }
    }
  }
  if (!duplicate) {
    return 0;
  }
  for (const auto& [_, match] : pieces) {
    if (!match) {
      return 0;
    }
  }
  return 1;
}

auto isNineGates(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  if (state.hands.at(player).open) {
    return 0;
  }
  if (isFullFlush(state, player, branch) == 0) {
    return 0;
  }
  if (state.hands.at(player).open) {
    return 0;
  }
  std::map<int, int> pieces;
  //NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  for (int i = 1; i < 10; i++) {
    pieces.at(i) = 0;
  }
  bool duplicate = false;
  for (const auto& piece : state.hands.at(player).live) {
    if (pieces.contains(piece.getPieceNum())) {
      if ((pieces[piece.getPieceNum()] != 0) && !piece.isTerminal()) {
        if (duplicate) {
          return 0;
        }
        duplicate = true;
      } else {
        pieces[piece.getPieceNum()]++;
      }
    }
  }
  if (!duplicate) {
    return 0;
  }
  for (const auto& [piece, count] : pieces) {
    //NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    if ((piece == 1 || piece == 9)) {
      if (count != 3) {
        return 0;
      }
    } else if (count != 1) {
      return 0;
    }
  }
  return 1;
}

auto isBlessingOfHeaven(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  if (state.hands.at(player).open) {
    return 0;
  }
  if (player != 0) {
    return 0;
  }
  if (state.hasRonned[0]) {
    return 0;
  }
  if (state.turnNum != 0) {
    return 0;
  }
  return 1;
}

auto isBlessingOfEarth(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  if (state.hands.at(player).open) {
    return 0;
  }
  if (state.turnNum > 3) {
    return 0;
  }
  if (state.lastCall >= 0) {
    return 0;
  }
  if (state.hasRonned.at(player)) {
    return 0;
  }
  return 1;
}

auto isFourConcealedPon(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  if (state.hands.at(player).open) {
    return 0;
  }
  int concealedPons = 0;
  for (const auto& node : branch) {
    if (node->type == Node::PonSet) {
      concealedPons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::ConcealedKan) {
      concealedPons++;
    }
  }
  if (concealedPons == 4) {
    return 1;
  }
  return 0;
}

auto isFourKans(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  int kans = 0;
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::Kan) {
      kans++;
    }
  }
  if (kans == 4) {
    return 1;
  }
  return 0;
}

auto isAllGreen(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isGreen()) {
      return 0;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (!meld.start.isGreen()) {
      return 0;
    }
  }
  return 1;
}

auto isAllTerminals(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isTerminal()) {
      return 0;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (!meld.start.isTerminal()) {
      return 0;
    }
  }
  return 1;
}

auto isAllHonors(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isHonor()) {
      return 0;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (!meld.start.isHonor()) {
      return 0;
    }
  }
  return 1;
}

auto isBigThreeDragons(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start.toUint8_t()) {
      case Piece::RED_DRAGON:
      case Piece::GREEN_DRAGON:
      case Piece::WHITE_DRAGON:
        break;
      default:
        continue;
    }
    if (node->type == Node::Pair) {
      return 0;
    }
    pons++;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    switch (meld.start.toUint8_t()) {
      case Piece::RED_DRAGON:
      case Piece::GREEN_DRAGON:
      case Piece::WHITE_DRAGON:
        break;
      default:
        continue;
    }
    pons++;
  }
  if (pons == 3) {
    return 1;
  }
  return 0;
}

auto isLittleFourWinds(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  bool pair = false;
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start.toUint8_t()) {
      case Piece::EAST_WIND:
      case Piece::SOUTH_WIND:
      case Piece::WEST_WIND:
      case Piece::NORTH_WIND:
        break;
      default:
        continue;
    }
    if (node->type != Node::Pair) {
      pons++;
    } else {
      if (pair) {
        return 0;
      }
      pair = true;
    }
  }
  if (!pair) {
    return 0;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    switch (meld.start.toUint8_t()) {
      case Piece::EAST_WIND:
      case Piece::SOUTH_WIND:
      case Piece::WEST_WIND:
      case Piece::NORTH_WIND:
        break;
      default:
        continue;
    }
    pons++;
  }
  if (pons == 3 && pair) {
    return 1;
  }
  return 0;
}

auto isBigFourWinds(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int {
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start.toUint8_t()) {
      case Piece::EAST_WIND:
      case Piece::SOUTH_WIND:
      case Piece::WEST_WIND:
      case Piece::NORTH_WIND:
        break;
      default:
        continue;
    }
    if (node->type == Node::Pair) {
      return 0;
    }
    pons++;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    switch (meld.start.toUint8_t()) {
      case Piece::EAST_WIND:
      case Piece::SOUTH_WIND:
      case Piece::WEST_WIND:
      case Piece::NORTH_WIND:
        break;
      default:
        continue;
    }
    pons++;
  }
  if (pons == 4) {
    return 1;
  }
  return 0;
}

auto isMaxBranches(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/) -> int {
  int start = state.hands.at(player).live[0].getPieceNum();
  int suit = state.hands.at(player).live[0].getSuit();
  std::array<int, 6> sets = {};
  const std::array<int, 6> final = {3, 3, 2, 2, 2, 2};
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.getSuit() != suit) {
      return 0;
    }
    if (piece.getPieceNum() < start || piece.getPieceNum() > start + 5) {
      return 0;
    }
    sets.at(piece.getPieceNum() - start)++;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.getSuit() != suit) {
      return 0;
    }
    if (meld.start.getPieceNum() < start || meld.start.getPieceNum() > start + 3) {
      return 0;
    }
    if (meld.type >= Meld::Kan) {
      if (meld.start.getPieceNum() != start && meld.start.getPieceNum() != start + 1) {
        return 0;
      }
      sets.at(meld.start.getPieceNum() - start) += 3;
    }
    if (meld.type == Meld::Pon) {
      sets.at(meld.start.getPieceNum() - start) += 3;
    }
    if (meld.type == Meld::Chi) {
      sets.at(meld.start.getPieceNum() - start)++;
      sets.at(meld.start.getPieceNum() - start + 1)++;
      sets.at(meld.start.getPieceNum() - start + 2)++;
    }
  }
  for (int i = 0; i < 6; i++) {
    if (sets.at(i) != final.at(i)) {
      return 0;
    }
  }
  return 1;
}

}  // namespace Mahjong
