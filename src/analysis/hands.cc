#include "hands.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "analysis/analysis.h"
#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/handnode.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/walls.h"
#include "types/winds.h"

namespace mahjong {

namespace {
const int kMaxSingles = 15;

int countSingles(std::vector<Piece> hand) {
  auto root = breakdownHand(std::move(hand));
  int min_singles = kMaxSingles;
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    int singles = 0;
    for (const auto& node : branch) {
      if (node->type == Node::kSingle) {
        singles++;
      }
    }
    min_singles = std::min(singles, min_singles);
  }
  return min_singles;
}
}  // namespace

int countPiece(const GameState& state, int player, Piece p) {
  int count = 0;
  for (const auto& piece : state.hands.at(player).live) {
    if (piece == p) {
      count++;
    }
  }
  return count;
}

const std::vector<mahjong::yakuFunc> kYakuFunctions = {isFullyConcealedHand,
                                                       isPinfu,
                                                       isPureDoubleChi,
                                                       isAllSimples,
                                                       isMixedTripleChi,
                                                       isPureStraight,
                                                       isWindOrDragonPon,
                                                       isOutsideHand,
                                                       isAfterAKan,
                                                       isRobbingAKan,
                                                       isBottomOfTheSea,
                                                       isSevenPairs,
                                                       isTriplePon,
                                                       isThreeConcealedPons,
                                                       isThreeKans,
                                                       isAllPons,
                                                       isHalfFlush,
                                                       isLittleThreeDragons,
                                                       isAllTerminalsAndHonors,
                                                       isTerminalsInAllSets,
                                                       isTwicePureDoubleChi,
                                                       isBlessingOfMan,
                                                       isFullFlush};

const std::vector<yakuFunc> kYakumanFunctions = {
    isThirteenOrphans, isNineGates,        isBlessingOfHeaven,
    isBlessingOfEarth, isFourConcealedPon, isFourKans,
    isAllGreen,        isAllTerminals,     isAllHonors,
    isBigThreeDragons, isLittleFourWinds,  isBigFourWinds,
    isMaxBranches};

Score scoreHand(const GameState& state, int player) {
  auto root = breakdownHand(state.hands.at(player).live);
  Score s;
  s.han = 0;
  s.yakuman = 0;
  s.fu = 0;
  if (!root->IsComplete() && (isThirteenOrphans(state, player) == 0) &&
      (isSevenPairs(state, player) == 0)) {
    return s;
  }
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    Score branchscore;
    for (const auto& node : branch) {
      if (node->type == Node::kSingle) {
        continue;
      }
    }

    for (const auto& yaku_function : kYakuFunctions) {
      branchscore.han += yaku_function(state, player, branch);
    }
    for (const auto& yaku_function : kYakumanFunctions) {
      branchscore.yakuman += yaku_function(state, player, branch);
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
        if (meld.type == Meld::kChi) {
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

const int kLowHanBasicPoints = 2000;
const int kManganBasicPoints = 2000;
const int kManganMinHan = 5;
const int kHanemanBasicPoints = 3000;
const int kHanemanMinHan = 6;
const int kBaimanBasicPoints = 4000;
const int kBaimanMinHan = 8;
const int kSanbaimanBasicPoints = 6000;
const int kSanbaimanMinHan = 11;
const int kYakumanBasicPoints = 8000;

int getBasicPoints(Score s) {
  if (s.yakuman > 0) {
    return s.yakuman * kYakumanBasicPoints;
  }
  if (s.han >= kManganMinHan) {
    if (s.han >= kSanbaimanMinHan) {
      return kSanbaimanBasicPoints;
    }
    if (s.han >= kBaimanMinHan) {
      return kBaimanBasicPoints;
    }
    if (s.han >= kHanemanMinHan) {
      return kHanemanBasicPoints;
    }
    if (s.han >= kSanbaimanMinHan) {
      return kSanbaimanBasicPoints;
    }
    if (s.han >= kManganMinHan) {
      return kManganBasicPoints;
    }
  }
  int p = s.fu * (2 << (1 + s.han));
  if (p > kLowHanBasicPoints) {
    return kLowHanBasicPoints;
  }
  return p;
}

const int kSevenpairs = 25;
const int kPinfuDiscard = 30;
const int kPinfuSelfdraw = 20;
const int kConcealedDiscard = 25;
const int kOpenOrSelfdraw = 20;

const int kSimplepon = 2;
const int kTermHonorpon = 4;
const int kSimplekan = 8;
const int kTermHonorkan = 16;

const int kConcealedMul = 2;
const int kCsimplepon = kSimplepon * kConcealedMul;
const int kCtermHonorpon = kTermHonorpon * kConcealedMul;
const int kCsimplekan = kSimplekan * kConcealedMul;
const int kCtermHonorkan = kTermHonorkan * kConcealedMul;

const int kDragonseatprevalentwind = 2;
const int kEdgeclosedpairwait = 2;
const int kSelfdraw = 2;
const int kOpenpinfu = 2;

const int kLowedgewait = 3;
const int kHighedgewait = 7;

const int kFuRounding = 10;

int getFu(const GameState& state, int player,
          const std::vector<const mahjong::Node*>& branch) {
  if (isSevenPairs(state, player, branch) != 0) {
    return kSevenpairs;
  }
  if (isPinfu(state, player, branch) != 0) {
    if (state.hasRonned.at(player)) {
      return kPinfuDiscard;
    }
    return kPinfuSelfdraw;
  }
  int fu = 0;
  if (!state.hands.at(player).open && state.hasRonned.at(player)) {
    fu = kConcealedDiscard;
  } else {
    fu = kOpenOrSelfdraw;
  }
  bool open = state.hands.at(player).open;
  if (isOpenPinfu(state, player, branch)) {
    fu += kOpenpinfu;
  } else if (!state.hasRonned.at(player)) {
    fu += kSelfdraw;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kKan) {
      if (!meld.start.isHonor() && !meld.start.isTerminal()) {
        fu += open ? kSimplekan : kCsimplekan;
      } else {
        fu += open ? kTermHonorkan : kCtermHonorkan;
      }
    }
  }
  for (const auto& node : branch) {
    if (node->type == Node::kPonSet) {
      if (!node->start.isHonor() && !node->start.isTerminal()) {
        fu += open ? kSimplepon : kCsimplepon;
      } else {
        fu += open ? kTermHonorpon : kCtermHonorpon;
      }
    }
    if (open) {
      continue;
    }
    if (node->type == Node::kPair) {
      if (node->start.isHonor()) {
        if (node->start == kGreenDragon || node->start == kRedDragon ||
            node->start == kWhiteDragon ||
            node->start == (state.roundNum > 3 ? kSouthWind : kEastWind)) {
          fu += kDragonseatprevalentwind;
        }
      }
      if (node->start == state.pendingPiece) {
        fu += kEdgeclosedpairwait;
      }
    }
    if (node->type == Node::kChiSet) {
      if (node->start + 1 == state.pendingPiece) {
        fu += kEdgeclosedpairwait;
      }
      if (node->start.getPieceNum() == 1 &&
          state.pendingPiece.getPieceNum() == kLowedgewait &&
          node->start.getSuit() == state.pendingPiece.getSuit()) {
        fu += kEdgeclosedpairwait;
      }
      if (node->start.getPieceNum() == kHighedgewait &&
          state.pendingPiece == node->start) {
        fu += kEdgeclosedpairwait;
      }
    }
  }
  if ((fu % kFuRounding) != 0) {
    return fu;
  }
  return fu +
         (kFuRounding - (fu % kFuRounding));  // rounding up to multiple of ten
}

bool isOpenPinfu(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch) {
  for (const auto& node : branch) {
    if (node->type == Node::kPonSet) {
      return false;
    }
    if (node->type == Node::kPair) {
      if (node->start == kRedDragon || node->start == kWhiteDragon ||
          node->start == kGreenDragon) {
        return false;
      }
      if (node->start == kSouthWind && state.roundNum > 3) {
        return false;
      }
      if (node->start == kEastWind && state.roundNum < 4) {
        return false;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type > Meld::kChi) {
      return false;
    }
  }
  auto waits = isInTenpai(state.hands.at(player).live, /*allWaits=*/true);
  return waits.size() != 1;
}

bool isComplete(const GameState& state, int player) {
  auto root = breakdownHand(state.hands.at(player).live);
  if (!root->IsComplete() && (isThirteenOrphans(state, player) == 0) &&
      (isSevenPairs(state, player) == 0)) {
    return false;
  }
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    for (const auto& node : branch) {
      if (node->type == Node::kSingle) {
        continue;
      }
    }

    for (const auto& yaku_function : kYakuFunctions) {
      if (yaku_function(state, player, branch) > 0) {
        return true;
      }
    }
    for (const auto& yaku_function : kYakumanFunctions) {
      if (yaku_function(state, player, branch) > 0) {
        return true;
      }
    }
  }

  return false;
}

const std::vector<Piece> kPieceSet{
    kOneBamboo,    kTwoBamboo,      kThreeBamboo,    kFourBamboo,
    kFiveBamboo,   kSixBamboo,      kSevenBamboo,    kEightBamboo,
    kNineBamboo,   kOnePin,         kTwoPin,         kThreePin,
    kFourPin,      kFivePin,        kSixPin,         kSevenPin,
    kEightPin,     kNinePin,        kOneCharacter,   kNineCharacter,
    kTwoCharacter, kThreeCharacter, kFourCharacter,  kFiveCharacter,
    kSixCharacter, kSevenCharacter, kEightCharacter, kWhiteDragon,
    kGreenDragon,  kRedDragon,      kEastWind,       kSouthWind,
    kNorthWind,    kWestWind};

// this is an extrememly inefficient algorithm but it's probably good enough for
// the frequency it needs to be ran
// will revisit if necessary
// assumption is 14 piece hand
std::vector<Piece> isInTenpai13Pieces(std::vector<Piece> hand, bool allWaits) {
  int min_singles = countSingles(hand);
  // These numbers were found by looking at a lot of handtrees and their single count
  if (min_singles > 5 || min_singles == 3 || min_singles == 0) {
    return {};
  }
  std::array<int8_t, Piece::kPiecesize> counts = {};
  std::vector<Piece> waits;
  for (const auto& p : hand) {
    counts.at(p.toUint8_t())++;
  }
  for (const auto& p : kPieceSet) {
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

const int kPiecesinahand = 14;

std::vector<Piece> isInTenpai(std::vector<Piece> hand, bool allWaits) {
  if (hand.empty()) {
    return {};
  }
  int min_singles = countSingles(hand);
  // These numbers are the same as above except one more piece means 1 higher on the single count
  if (min_singles > 6 || min_singles == 1 || min_singles == 4 ||
      min_singles == 0) {
    return {};
  }
  std::array<bool, Piece::kPiecesize> removedbefore = {};
  std::vector<Piece> waits;
  for (int i = 0; i < kPiecesinahand; i++) {
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

// see above comment
std::vector<Piece> getRiichiDiscard(std::vector<Piece> hand) {
  if (hand.empty()) {
    return {};
  }
  std::array<int8_t, Piece::kPiecesize> counts = {};
  std::array<bool, Piece::kPiecesize> removedbefore = {};
  std::vector<Piece> remove_me;
  for (const auto& p : hand) {
    counts.at(p.toUint8_t())++;
  }
  for (int i = 0; i < kPiecesinahand; i++) {
    Piece removed = hand.front();
    hand.erase(hand.begin());
    if (removedbefore.at(removed.toUint8_t())) {
      hand.push_back(removed);
      continue;
    }
    removedbefore.at(removed.toUint8_t()) = true;
    for (const auto& p : kPieceSet) {
      if (counts.at(p.toUint8_t()) == 4 || p == removed) {
        continue;
      }
      hand.push_back(p);
      auto root = breakdownHand(hand);
      if (root->IsComplete()) {
        remove_me.push_back(removed);
      }

      hand.pop_back();
    }
    hand.push_back(removed);
  }
  return remove_me;
}

const int kOneroundofturns = 4;
int isRiichi(const GameState& state, int player,
             const std::vector<const mahjong::Node*>& /*unused*/) {
  if (state.hands.at(player).riichi) {
    int han = 1;
    if (state.turnNum < 4 && state.lastCall < 0) {
      han++;
    }
    if (state.turnNum - state.hands.at(player).riichiRound <=
            kOneroundofturns &&
        state.lastCall < state.hands.at(player).riichiRound) {
      han++;
    }
    return han;
  }
  return 0;
}

int isFullyConcealedHand(const GameState& state, int player,
                         const std::vector<const mahjong::Node*>& /*unused*/) {
  if (state.currentPlayer == player && !state.hands.at(player).open &&
      state.walls.GetRemainingPieces() > 0) {
    return 1;
  }
  return 0;
}

int isPinfu(const GameState& state, int player,
            const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return 0;
  }
  for (const auto& node : branch) {
    if (node->type == Node::kPonSet) {
      return 0;
    }
    if (node->type == Node::kPair) {
      if (node->start == kRedDragon || node->start == kWhiteDragon ||
          node->start == kGreenDragon) {
        return 0;
      }
      if (node->start == kSouthWind && state.roundNum > 3) {
        return 0;
      }
      if (node->start == kEastWind && state.roundNum < 4) {
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

int isPureDoubleChi(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return 0;
  }
  if (isTwicePureDoubleChi(state, player, branch) != 0) {
    return 0;
  }
  for (size_t i = 0; i < branch.size(); i++) {
    if (branch.at(i)->type != Node::kChiSet) {
      continue;
    }
    for (size_t j = 0; j < branch.size(); j++) {
      if (i == j) {
        continue;
      }
      if (branch.at(i)->type == branch[j]->type &&
          branch.at(i)->start == branch[j]->start) {
        return 1;
      }
    }
  }
  return 0;
}

int isAllSimples(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*unused*/) {
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

const int kPiecesinasuit = 9;
int isMixedTripleChi(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& branch) {
  std::array<bool, kPiecesinasuit> bamboo_chi = {};
  std::array<bool, kPiecesinasuit> char_chi = {};
  std::array<bool, kPiecesinasuit> pin_chi = {};
  for (const auto& node : branch) {
    if (node->type == Node::kChiSet) {
      if (node->start.getSuit() == Piece::Type::kBambooSuit) {
        bamboo_chi.at(node->start.getPieceNum()) = true;
      }
      if (node->start.getSuit() == Piece::Type::kCharacterSuit) {
        char_chi.at(node->start.getPieceNum()) = true;
      }
      if (node->start.getSuit() == Piece::Type::kPinSuit) {
        pin_chi.at(node->start.getPieceNum()) = true;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kChi) {
      if (meld.start.getSuit() == Piece::Type::kBambooSuit) {
        bamboo_chi.at(meld.start.getPieceNum()) = true;
      }
      if (meld.start.getSuit() == Piece::Type::kCharacterSuit) {
        char_chi.at(meld.start.getPieceNum()) = true;
      }
      if (meld.start.getSuit() == Piece::Type::kPinSuit) {
        pin_chi.at(meld.start.getPieceNum()) = true;
      }
    }
  }
  for (int i = 0; i < kPiecesinasuit; i++) {
    if (bamboo_chi.at(i) && char_chi.at(i) && pin_chi.at(i)) {
      return state.hands.at(player).open ? 1 : 2;
    }
  }
  return 0;
}

const int kFirstchistart = 1;
const int kSecondchistart = 4;
const int kThirdchistart = 7;
int isPureStraight(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch) {
  std::array<bool, 3> bamboo_chi = {};
  std::array<bool, 3> char_chi = {};
  std::array<bool, 3> pin_chi = {};
  for (const auto& node : branch) {
    if (node->type == Node::kChiSet) {
      int ind = 0;
      if (node->start.getPieceNum() == kFirstchistart) {
        ind = 0;
      } else if (node->start.getPieceNum() == kSecondchistart) {
        ind = 1;
      } else if (node->start.getPieceNum() == kThirdchistart) {
        ind = 2;
      } else {
        continue;
      }
      if (node->start.getSuit() == Piece::Type::kBambooSuit) {
        bamboo_chi.at(ind) = true;
      }
      if (node->start.getSuit() == Piece::Type::kCharacterSuit) {
        char_chi.at(ind) = true;
      }
      if (node->start.getSuit() == Piece::Type::kPinSuit) {
        pin_chi.at(ind) = true;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kChi) {
      int ind = 0;
      if (meld.start.getPieceNum() == kFirstchistart) {
        ind = 0;
      } else if (meld.start.getPieceNum() == kSecondchistart) {
        ind = 1;
      } else if (meld.start.getPieceNum() == kThirdchistart) {
        ind = 2;
      } else {
        continue;
      }
      if (meld.start.getSuit() == Piece::Type::kBambooSuit) {
        bamboo_chi.at(ind) = true;
      }
      if (meld.start.getSuit() == Piece::Type::kCharacterSuit) {
        char_chi.at(ind) = true;
      }
      if (meld.start.getSuit() == Piece::Type::kPinSuit) {
        pin_chi.at(ind) = true;
      }
    }
  }
  int han = state.hands.at(player).open ? 1 : 2;
  if (bamboo_chi[0] && bamboo_chi[1] && bamboo_chi[2]) {
    return han;
  }
  if (char_chi[0] && char_chi[1] && char_chi[2]) {
    return han;
  }
  if (pin_chi[0] && pin_chi[1] && pin_chi[2]) {
    return han;
  }
  return 0;
}

int isWindOrDragonPon(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& branch) {
  std::vector<Piece> matches{
      kWhiteDragon, kGreenDragon, kRedDragon,
      Piece::fromWind(GetSeat(state.roundNum, player)),
      Piece::fromWind(state.roundNum > 3 ? kSouth : kEast)};
  int han = 0;
  for (const auto& match : matches) {
    bool is_match = false;
    for (const auto& node : branch) {
      if (node->type == Node::kPonSet && node->start == match) {
        is_match = true;
        break;
      }
    }
    for (const auto& meld : state.hands.at(player).melds) {
      if (meld.type >= Meld::kPon && meld.start == match) {
        is_match = true;
        break;
      }
    }
    if (is_match) {
      han++;
    }
  }
  return han;
}

int isOutsideHand(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& branch) {
  if ((isTerminalsInAllSets(state, player, branch) != 0) ||
      (isAllTerminalsAndHonors(state, player, branch) != 0)) {
    return 0;
  }
  bool chi = false;
  for (const auto& node : branch) {
    if (node->type == Node::kChiSet) {
      if (node->start.isTerminal() || (node->start + 2).isTerminal()) {
        chi = true;
      } else {
        return 0;
      }
    } else {
      if (!node->start.isTerminal() && !node->start.isHonor()) {
        return 0;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kChi) {
      if (meld.start.isTerminal() || (meld.start + 2).isTerminal()) {
        chi = true;
      } else {
        return 0;
      }
    } else {
      if (!meld.start.isTerminal() && !meld.start.isHonor()) {
        return 0;
      }
    }
  }
  if (!chi) {
    return 0;
  }
  return state.hands.at(state.currentPlayer).open ? 1 : 2;
}

int isAfterAKan(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*unused*/) {
  if (state.currentPlayer != player) {
    return 0;
  }
  if (state.prevState == Replacement) {
    return 1;
  }
  return 0;
}

int isRobbingAKan(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& /*unused*/) {
  if (!state.hasRonned.at(player)) {
    return 0;
  }
  if (state.nextState == KanDiscard) {
    return 1;
  }
  return 0;
}

int isBottomOfTheSea(const GameState& state, int /*unused*/,
                     const std::vector<const mahjong::Node*>& /*unused*/) {
  if (state.walls.GetRemainingPieces() == 0) {
    return 1;
  }
  return 0;
}

int isSevenPairs(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*unused*/) {
  if (state.hands.at(player).open) {
    return 0;
  }
  for (size_t i = 0; i < state.hands.at(player).live.size(); i += 2) {
    if (state.hands.at(player).live.at(i) !=
        state.hands.at(player).live[i + 1]) {
      return 0;
    }
  }
  return 2;
}

int isTriplePon(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& branch) {
  std::array<bool, kPiecesinasuit> bamboo_pon = {};
  std::array<bool, kPiecesinasuit> char_pon = {};
  std::array<bool, kPiecesinasuit> pin_pon = {};
  for (const auto& node : branch) {
    if (node->type == Node::kPonSet) {
      if (node->start.getSuit() == Piece::Type::kBambooSuit) {
        bamboo_pon.at(node->start.getPieceNum() - 1) = true;
      }
      if (node->start.getSuit() == Piece::Type::kCharacterSuit) {
        char_pon.at(node->start.getPieceNum() - 1) = true;
      }
      if (node->start.getSuit() == Piece::Type::kPinSuit) {
        pin_pon.at(node->start.getPieceNum() - 1) = true;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::kPon) {
      if (meld.start.getSuit() == Piece::Type::kBambooSuit) {
        bamboo_pon.at(meld.start.getPieceNum() - 1) = true;
      }
      if (meld.start.getSuit() == Piece::Type::kCharacterSuit) {
        char_pon.at(meld.start.getPieceNum() - 1) = true;
      }
      if (meld.start.getSuit() == Piece::Type::kPinSuit) {
        pin_pon.at(meld.start.getPieceNum() - 1) = true;
      }
    }
  }
  for (int i = 0; i < kPiecesinasuit; i++) {
    if (bamboo_pon.at(i) && char_pon.at(i) && pin_pon.at(i)) {
      return 2;
    }
  }
  return 0;
}

int isThreeConcealedPons(const GameState& state, int player,
                         const std::vector<const mahjong::Node*>& branch) {
  int concealed_pons = 0;
  for (const auto& node : branch) {
    if (node->type == Node::kPonSet) {
      concealed_pons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kConcealedKan) {
      concealed_pons++;
    }
  }
  if (concealed_pons >= 3) {
    return 2;
  }
  return 0;
}

int isThreeKans(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*unused*/) {
  int kans = 0;
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::kKan) {
      kans++;
    }
  }
  if (kans >= 3) {
    return 2;
  }
  return 0;
}

int isAllPons(const GameState& state, int player,
              const std::vector<const mahjong::Node*>& branch) {
  for (const auto& node : branch) {
    if (node->type != Node::kPonSet) {
      return 0;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kChi) {
      return 0;
    }
  }
  return 2;
}

int isHalfFlush(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& branch) {
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

int isLittleThreeDragons(const GameState& state, int player,
                         const std::vector<const mahjong::Node*>& branch) {
  bool pair = false;
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start.toUint8_t()) {
      case Piece::Type::kRedDragon:
      case Piece::Type::kGreenDragon:
      case Piece::Type::kWhiteDragon:
        break;
      default:
        continue;
    }
    if (node->type != Node::kPair) {
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
      case Piece::Type::kRedDragon:
      case Piece::Type::kGreenDragon:
      case Piece::Type::kWhiteDragon:
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

int isAllTerminalsAndHonors(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*unused*/) {
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

int isTerminalsInAllSets(const GameState& state, int player,
                         const std::vector<const mahjong::Node*>& branch) {
  if (isFullFlush(state, player, branch) != 0) {
    return 0;
  }
  bool chi = false;
  for (const auto& node : branch) {
    if (node->type == Node::kChiSet) {
      if (node->start.isTerminal() || (node->start + 2).isTerminal()) {
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
    if (meld.type == Meld::kChi) {
      if (meld.start.isTerminal() || (meld.start + 2).isTerminal()) {
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

int isTwicePureDoubleChi(const GameState& state, int player,
                         const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return 0;
  }
  int twice = 0;
  for (size_t i = 0; i < branch.size(); i++) {
    if (branch.at(i)->type != Node::kChiSet) {
      continue;
    }
    for (size_t j = 0; j < branch.size(); j++) {
      if (i == j) {
        continue;
      }
      if (branch.at(i)->type == branch[j]->type &&
          branch.at(i)->start == branch[j]->start) {
        twice++;
      }
    }
  }
  if (twice == 2) {
    return 3;
  }
  return 0;
}

int isBlessingOfMan(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& /*unused*/) {
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
    return 5;
  }
  return 0;
}

int isFullFlush(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*unused*/) {
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
  return state.hands.at(player).open ? 5 : 6;
}

int isThirteenOrphans(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& /*unused*/) {
  if (state.hands.at(player).open) {
    return 0;
  }
  std::map<Piece, bool> pieces = {
      {kOneCharacter, false}, {kNineCharacter, false}, {kOnePin, false},
      {kNinePin, false},      {kOneBamboo, false},     {kNineBamboo, false},
      {kEastWind, false},     {kSouthWind, false},     {kWestWind, false},
      {kNorthWind, false},    {kRedDragon, false},     {kWhiteDragon, false},
      {kGreenDragon, false}};
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

int isNineGates(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& branch) {
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

int isBlessingOfHeaven(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& /*unused*/) {
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

int isBlessingOfEarth(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& /*unused*/) {
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

int isFourConcealedPon(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return 0;
  }
  int concealed_pons = 0;
  for (const auto& node : branch) {
    if (node->type == Node::kPonSet) {
      concealed_pons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kConcealedKan) {
      concealed_pons++;
    }
  }
  if (concealed_pons == 4) {
    return 1;
  }
  return 0;
}

int isFourKans(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& /*unused*/) {
  int kans = 0;
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::kKan) {
      kans++;
    }
  }
  if (kans == 4) {
    return 1;
  }
  return 0;
}

int isAllGreen(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& /*unused*/) {
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

int isAllTerminals(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*unused*/) {
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

int isAllHonors(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*unused*/) {
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

int isBigThreeDragons(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& branch) {
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start.toUint8_t()) {
      case Piece::Type::kRedDragon:
      case Piece::Type::kGreenDragon:
      case Piece::Type::kWhiteDragon:
        break;
      default:
        continue;
    }
    if (node->type == Node::kPair) {
      return 0;
    }
    pons++;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    switch (meld.start.toUint8_t()) {
      case Piece::Type::kRedDragon:
      case Piece::Type::kGreenDragon:
      case Piece::Type::kWhiteDragon:
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

int isLittleFourWinds(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& branch) {
  bool pair = false;
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start.toUint8_t()) {
      case Piece::Type::kEastWind:
      case Piece::Type::kSouthWind:
      case Piece::Type::kWestWind:
      case Piece::Type::kNorthWind:
        break;
      default:
        continue;
    }
    if (node->type != Node::kPair) {
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
      case Piece::Type::kEastWind:
      case Piece::Type::kSouthWind:
      case Piece::Type::kWestWind:
      case Piece::Type::kNorthWind:
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

int isBigFourWinds(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch) {
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start.toUint8_t()) {
      case Piece::Type::kEastWind:
      case Piece::Type::kSouthWind:
      case Piece::Type::kWestWind:
      case Piece::Type::kNorthWind:
        break;
      default:
        continue;
    }
    if (node->type == Node::kPair) {
      return 0;
    }
    pons++;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    switch (meld.start.toUint8_t()) {
      case Piece::Type::kEastWind:
      case Piece::Type::kSouthWind:
      case Piece::Type::kWestWind:
      case Piece::Type::kNorthWind:
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

int isMaxBranches(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& /*unused*/) {
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
    if (meld.start.getPieceNum() < start ||
        meld.start.getPieceNum() > start + 3) {
      return 0;
    }
    if (meld.type >= Meld::kKan) {
      if (meld.start.getPieceNum() != start &&
          meld.start.getPieceNum() != start + 1) {
        return 0;
      }
      sets.at(meld.start.getPieceNum() - start) += 3;
    }
    if (meld.type == Meld::kPon) {
      sets.at(meld.start.getPieceNum() - start) += 3;
    }
    if (meld.type == Meld::kChi) {
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

}  // namespace mahjong
