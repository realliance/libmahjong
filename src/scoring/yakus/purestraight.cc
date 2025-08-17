#include "scoring/yakus/purestraight.h"

#include <array>
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
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

}  // namespace mahjong::yaku
