#include "scoring/yakus/purestraight.h"

#include <array>
#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isPureStraight(const GameState& /* state */, const Hand& hand,
                    const std::vector<const mahjong::Node*>& branch) {
  const int k_firstchistart = 1;
  const int k_secondchistart = 4;
  const int k_thirdchistart = 7;
  std::array<bool, 3> bamboo_chi = {};
  std::array<bool, 3> char_chi = {};
  std::array<bool, 3> pin_chi = {};
  for (const auto* node : branch) {
    if (node->type() == SetType::kChi) {
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
  for (const auto& meld : hand.melds_range()) {
    if (meld.type == SetType::kChi) {
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

REGISTER_YAKU({
    .id = "purestraight",
    .name = "Pure Straight",
    .type = Yaku::kBonusWhenClosed,
    .value = 1,
    .is_yaku_func = yaku::isPureStraight,
});
}  // namespace mahjong::yaku
