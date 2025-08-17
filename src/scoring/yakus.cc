#include "scoring/yakus.h"
#include "analysis/util.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <map>
#include <set>
#include <vector>

#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/meld.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/statefunction.h"
#include "types/walls.h"
#include "types/winds.h"

namespace mahjong {
bool isRiichi(const GameState& state, int player,
              const std::vector<const mahjong::Node*>& /*branch*/) {
  return state.hands.at(player).riichi && !state.hands.at(player).open &&
         (state.turnNum > 4 || state.lastCall < 0);
}

bool isDoubleRiichi(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& /*branch*/) {
  return state.hands.at(player).riichi && !state.hands.at(player).open &&
         (state.turnNum < 4 && state.lastCall < 0);
}

bool isIppatsu(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& /*branch*/) {
  return (isRiichi(state, player) || isDoubleRiichi(state, player)) &&
         (state.turnNum - state.hands.at(player).riichiRound <= 4 &&
          state.lastCall < state.hands.at(player).riichiRound);
}

bool isFullyConcealedHand(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& /*branch*/) {
  return state.currentPlayer == player && !state.hands.at(player).open &&
         state.walls.GetRemainingPieces() > 0;
}

bool isPinfu(const GameState& state, int player,
             const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  for (const auto& node : branch) {
    switch (node->type()) {
      case Node::kPonSet:
      case Node::kSingle:
      case Node::kError:
        return false;

      case Node::kChiSet:
      case Node::kRoot:
        continue;
      case Node::kPair: {
        if (node->start() == kRedDragon || node->start() == kWhiteDragon ||
            node->start() == kGreenDragon) {
          return false;
        }
        if (node->start() == kSouthWind && state.roundNum > 3) {
          return false;
        }
        if (node->start() == kEastWind && state.roundNum < 4) {
          return false;
        }
        if (node->start() == Piece::fromWind(GetSeat(state.roundNum, player))) {
          return false;
        }
      }
    }
  }
  std::vector<Piece> hand = state.hands.at(player).live;
  hand.erase(std::find(hand.begin(), hand.end(), state.pendingPiece));
  return isInTenpai13Pieces(hand, /*allWaits=*/true).size() > 1;
}

bool isPureDoubleChi(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  for (size_t i = 0; i < branch.size(); i++) {
    if (branch.at(i)->type() != Node::kChiSet) {
      continue;
    }
    for (size_t j = 0; j < branch.size(); j++) {
      if (i == j) {
        continue;
      }
      if (branch.at(i)->type() == branch[j]->type() &&
          branch.at(i)->start() == branch[j]->start()) {
        return true;
      }
    }
  }
  return false;
}

bool isAllSimples(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.isHonor() || piece.isTerminal()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.isHonor() || meld.start.isTerminal()) {
      return false;
    }
  }

  return true;
}

bool isMixedTripleChi(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& branch) {
  const int k_piecesinasuit = 9;
  std::array<bool, k_piecesinasuit> bamboo_chi = {};
  std::array<bool, k_piecesinasuit> char_chi = {};
  std::array<bool, k_piecesinasuit> pin_chi = {};
  for (const auto& node : branch) {
    if (node->type() == Node::kChiSet) {
      if (node->start().getSuit() == Piece::Type::kBambooSuit) {
        bamboo_chi.at(node->start().getPieceNum()) = true;
      }
      if (node->start().getSuit() == Piece::Type::kCharacterSuit) {
        char_chi.at(node->start().getPieceNum()) = true;
      }
      if (node->start().getSuit() == Piece::Type::kPinSuit) {
        pin_chi.at(node->start().getPieceNum()) = true;
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
  for (int i = 0; i < k_piecesinasuit; i++) {
    if (bamboo_chi.at(i) && char_chi.at(i) && pin_chi.at(i)) {
      return true;
    }
  }
  return false;
}

bool isPureStraight(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& branch) {
  const int k_firstchistart = 1;
  const int k_secondchistart = 4;
  const int k_thirdchistart = 7;
  std::array<bool, 3> bamboo_chi = {};
  std::array<bool, 3> char_chi = {};
  std::array<bool, 3> pin_chi = {};
  for (const auto& node : branch) {
    if (node->type() == Node::kChiSet) {
      int ind = 0;
      if (node->start().getPieceNum() == k_firstchistart) {
        ind = 0;
      } else if (node->start().getPieceNum() == k_secondchistart) {
        ind = 1;
      } else if (node->start().getPieceNum() == k_thirdchistart) {
        ind = 2;
      } else {
        continue;
      }
      if (node->start().getSuit() == Piece::Type::kBambooSuit) {
        bamboo_chi.at(ind) = true;
      }
      if (node->start().getSuit() == Piece::Type::kCharacterSuit) {
        char_chi.at(ind) = true;
      }
      if (node->start().getSuit() == Piece::Type::kPinSuit) {
        pin_chi.at(ind) = true;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kChi) {
      int ind = 0;
      if (meld.start.getPieceNum() == k_firstchistart) {
        ind = 0;
      } else if (meld.start.getPieceNum() == k_secondchistart) {
        ind = 1;
      } else if (meld.start.getPieceNum() == k_thirdchistart) {
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
  if (bamboo_chi[0] && bamboo_chi[1] && bamboo_chi[2]) {
    return true;
  }
  if (char_chi[0] && char_chi[1] && char_chi[2]) {
    return true;
  }
  if (pin_chi[0] && pin_chi[1] && pin_chi[2]) {
    return true;
  }
  return false;
}

namespace {
bool findPon(const GameState& state, int player,
             const std::vector<const mahjong::Node*>& branch,
             const Piece& piece) {
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet && node->start() == piece) {
      return true;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::kPon && meld.start == piece) {
      return true;
    }
  }
  return false;
}

}  // namespace

bool isSeatWind(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch,
                 Piece::fromWind(GetSeat(state.roundNum, player)));
}

bool isPrevalentWind(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch,
                 Piece::fromWind(state.roundNum > 3 ? kSouth : kEast));
}

bool isGreenDragon(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch, kGreenDragon);
}

bool isRedDragon(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch, kRedDragon);
}

bool isWhiteDragon(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch, kWhiteDragon);
}

bool isOutsideHand(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch) {
  bool chi = false;
  for (const auto& node : branch) {
    if (node->type() == Node::kChiSet) {
      if (node->start().isTerminal() || (node->start() + 2).isTerminal()) {
        chi = true;
      } else {
        return false;
      }
    } else {
      if (!node->start().isTerminal() && !node->start().isHonor()) {
        return false;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kChi) {
      if (meld.start.isTerminal() || (meld.start + 2).isTerminal()) {
        chi = true;
      } else {
        return false;
      }
    } else {
      if (!meld.start.isTerminal() && !meld.start.isHonor()) {
        return false;
      }
    }
  }
  // Terminals in all Sets and All Terminals and Honors are more valuable and score instead of Outside Hand.
  return chi && !isTerminalsInAllSets(state, player, branch) &&
         !isAllTerminalsAndHonors(state, player);
}

bool isAfterAKan(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.currentPlayer != player) {
    return false;
  }
  if (state.prevState == StateFunctionType::kReplacement) {
    return true;
  }
  return false;
}

bool isRobbingAKan(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*branch*/) {
  if (!state.hasRonned.at(player)) {
    return false;
  }
  if (state.nextState == StateFunctionType::kKanDiscard) {
    return true;
  }
  return false;
}

bool isBottomOfTheSea(const GameState& state, int /*player*/,
                      const std::vector<const mahjong::Node*>& /*branch*/) {
  return state.walls.GetRemainingPieces() == 0;
}

bool isSevenPairs(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& branch) {
  if (state.hands[player].open) {
    return false;
  }
  std::set<Piece> pairs;
  for (const auto& node : branch) {
    switch (node->type()) {
      case Node::kError:
      case Node::kChiSet:
      case Node::kPonSet:
      case Node::kSingle:
        return false;
      case Node::kPair:
        pairs.insert(node->start());
      case Node::kRoot:
        break;
    }
  }
  return pairs.size() == 7;
}

bool isTriplePon(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch) {
  std::array<bool, 9> bamboo_pon = {};
  std::array<bool, 9> char_pon = {};
  std::array<bool, 9> pin_pon = {};
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet) {
      if (node->start().getSuit() == Piece::Type::kBambooSuit) {
        bamboo_pon.at(node->start().getPieceNum() - 1) = true;
      }
      if (node->start().getSuit() == Piece::Type::kCharacterSuit) {
        char_pon.at(node->start().getPieceNum() - 1) = true;
      }
      if (node->start().getSuit() == Piece::Type::kPinSuit) {
        pin_pon.at(node->start().getPieceNum() - 1) = true;
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
  for (int i = 0; i < 9; i++) {
    if (bamboo_pon.at(i) && char_pon.at(i) && pin_pon.at(i)) {
      return true;
    }
  }
  return false;
}

bool isThreeConcealedPons(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch) {
  int concealed_pons = 0;
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet) {
      concealed_pons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kConcealedKan) {
      concealed_pons++;
    }
  }
  return concealed_pons >= 3;
}

bool isThreeKans(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  int kans = 0;
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::kKan) {
      kans++;
    }
  }
  return kans >= 3;
}

bool isAllPons(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& branch) {
  int pons = 0;
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet) {
      pons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kKan || meld.type == Meld::kPon ||
        meld.type == Meld::kConcealedKan) {
      pons++;
    }
  }
  return pons == 4;
}

bool isHalfFlush(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  const int suit = state.hands.at(player).live.front().getSuit();
  bool honors = false;
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.isHonor()) {
      honors = true;
      continue;
    }
    if (suit != piece.getSuit()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.isHonor()) {
      honors = true;
      continue;
    }
    if (suit != meld.start.getSuit()) {
      return false;
    }
  }
  // Full Flush scores instead of Half Flush.
  return honors && !isFullFlush(state, player);
}

bool isLittleThreeDragons(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch) {
  bool pair = false;
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start().toUint8_t()) {
      case Piece::Type::kRedDragon:
      case Piece::Type::kGreenDragon:
      case Piece::Type::kWhiteDragon:
        break;
      default:
        continue;
    }
    if (node->type() == Node::kPonSet) {
      pons++;
    }
    if (node->type() == Node::kPair) {
      pair = true;
    }
  }
  if (!pair) {
    return false;
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
  return pons == 2 && pair;
}

bool isAllTerminalsAndHonors(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isHonor() && !piece.isTerminal()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (!meld.start.isHonor() && !meld.start.isTerminal()) {
      return false;
    }
  }
  return true;
}

bool isTerminalsInAllSets(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch) {
  for (const auto& node : branch) {
    switch (node->type()) {
      case Node::kError:
      case Node::kSingle:
        return false;
      case Node::kRoot:
        break;
      case Node::kChiSet:
        if (!node->start().isTerminal() && !(node->start() + 2).isTerminal()) {
          return false;
        }
        break;
      case Node::kPair:
      case Node::kPonSet:
        if (!node->start().isTerminal()) {
          return false;
        }
        break;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    switch (meld.type) {
      case Meld::kChi:
        if (!meld.start.isTerminal() && !(meld.start + 2).isTerminal()) {
          return false;
        }
        break;
      case Meld::kKan:
      case Meld::kConcealedKan:
      case Meld::kPon:
        if (!meld.start.isTerminal()) {
          return false;
        }
        break;
    }
  }
  return true;
}

bool isTwicePureDoubleChi(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  int pairs = 0;
  for (size_t i = 0; i < branch.size(); i++) {
    if (branch.at(i)->type() != Node::kChiSet) {
      continue;
    }
    for (size_t j = i + 1; j < branch.size(); j++) {
      if (branch.at(j)->type() == Node::kChiSet &&
          branch.at(i)->start() == branch.at(j)->start()) {
        pairs++;
      }
    }
  }
  return pairs == 2;
}

bool isBlessingOfMan(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).open) {
    return false;
  }
  if (state.turnNum > player) {
    return false;
  }
  if (state.lastCall >= 0) {
    return false;
  }
  if (state.hasRonned.at(player)) {
    return true;
  }
  return false;
}

bool isFullFlush(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).live.front().isHonor()) {
    return false;
  }
  const int suit = state.hands.at(player).live.front().getSuit();
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.getSuit() != suit) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.getSuit() != suit) {
      return false;
    }
  }
  return true;
}

bool isThirteenOrphans(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).open) {
    return false;
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
    return false;
  }
  for (const auto& [_, match] : pieces) {
    if (!match) {
      return false;
    }
  }
  return true;
}

bool isNineGates(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  if (!isFullFlush(state, player, branch)) {
    return false;
  }
  if (state.hands.at(player).open) {
    return false;
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
          return false;
        }
        duplicate = true;
      } else {
        pieces[piece.getPieceNum()]++;
      }
    }
  }
  if (!duplicate) {
    return false;
  }
  for (const auto& [piece, count] : pieces) {
    if ((piece == 1 || piece == 9)) {
      if (count != 3) {
        return false;
      }
    } else if (count != 1) {
      return false;
    }
  }
  return true;
}

bool isBlessingOfHeaven(const GameState& state, int player,
                        const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).open) {
    return false;
  }
  if (player != 0) {
    return false;
  }
  if (state.hasRonned[0]) {
    return false;
  }
  if (state.turnNum != 0) {
    return false;
  }
  return true;
}

bool isBlessingOfEarth(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).open) {
    return false;
  }
  if (state.turnNum > 3) {
    return false;
  }
  if (state.lastCall >= 0) {
    return false;
  }
  if (state.hasRonned.at(player)) {
    return false;
  }
  return true;
}

bool isFourConcealedPon(const GameState& state, int player,
                        const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  int concealed_pons = 0;
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet) {
      concealed_pons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kConcealedKan) {
      concealed_pons++;
    }
  }
  return concealed_pons == 4;
}

bool isFourKans(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*branch*/) {
  int kans = 0;
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::kKan) {
      kans++;
    }
  }
  return kans == 4;
}

bool isAllGreen(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isGreen()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (!meld.start.isGreen()) {
      return false;
    }
  }
  return true;
}

bool isAllTerminals(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isTerminal()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (!meld.start.isTerminal()) {
      return false;
    }
  }
  return true;
}

bool isAllHonors(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isHonor()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (!meld.start.isHonor()) {
      return false;
    }
  }
  return true;
}

bool isBigThreeDragons(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& branch) {
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start().toUint8_t()) {
      case Piece::Type::kRedDragon:
      case Piece::Type::kGreenDragon:
      case Piece::Type::kWhiteDragon:
        break;
      default:
        continue;
    }
    if (node->type() == Node::kPair) {
      return false;
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
  return pons == 3;
}

bool isLittleFourWinds(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& branch) {
  bool pair = false;
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start().toUint8_t()) {
      case Piece::Type::kEastWind:
      case Piece::Type::kSouthWind:
      case Piece::Type::kWestWind:
      case Piece::Type::kNorthWind:
        break;
      default:
        continue;
    }
    if (node->type() != Node::kPair) {
      pons++;
    } else {
      if (pair) {
        return false;
      }
      pair = true;
    }
  }
  if (!pair) {
    return false;
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
  return pons == 3 && pair;
}

bool isBigFourWinds(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& branch) {
  int pons = 0;
  for (const auto& node : branch) {
    switch (node->start().toUint8_t()) {
      case Piece::Type::kEastWind:
      case Piece::Type::kSouthWind:
      case Piece::Type::kWestWind:
      case Piece::Type::kNorthWind:
        break;
      default:
        continue;
    }
    if (node->type() == Node::kPair) {
      return false;
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
  return pons == 4;
}

bool isMaxBranches(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*branch*/) {
  const int start = state.hands.at(player).live[0].getPieceNum();
  const int suit = state.hands.at(player).live[0].getSuit();
  std::array<int, 6> sets = {};
  const std::array<int, 6> final = {3, 3, 2, 2, 2, 2};
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.getSuit() != suit) {
      return false;
    }
    if (piece.getPieceNum() < start || piece.getPieceNum() > start + 5) {
      return false;
    }
    sets.at(piece.getPieceNum() - start)++;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.getSuit() != suit) {
      return false;
    }
    if (meld.start.getPieceNum() < start ||
        meld.start.getPieceNum() > start + 3) {
      return false;
    }
    if (meld.type >= Meld::kKan) {
      if (meld.start.getPieceNum() != start &&
          meld.start.getPieceNum() != start + 1) {
        return false;
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
      return false;
    }
  }
  return true;
}

}  // namespace mahjong
