#include "scoring/yakus/mixedtriplechi.h"

#include <array>
#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isMixedTripleChi(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& branch) {
  const int k_piecesinasuit = 9;
  std::array<bool, k_piecesinasuit> bamboo_chi = {};
  std::array<bool, k_piecesinasuit> char_chi = {};
  std::array<bool, k_piecesinasuit> pin_chi = {};
  for (const auto* node : branch) {
    if (node->type() == SetType::kChi) {
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
  const Hand& hand = state.hands[player];
  for (const auto& meld : hand.melds_range()) {
    if (meld.type == SetType::kChi) {
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

REGISTER_YAKU({
    .id = "mixedtriplechi",
    .name = "Mixed Triple Chi",
    .type = Yaku::kBonusWhenClosed,
    .value = 1,
    .is_yaku_func = yaku::isMixedTripleChi,
});
}  // namespace mahjong::yaku
