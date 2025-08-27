#include "scoring/yakus/triplepon.h"

#include <array>
#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isTriplePon(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch) {
  std::array<bool, 9> bamboo_pon = {};
  std::array<bool, 9> char_pon = {};
  std::array<bool, 9> pin_pon = {};
  for (const auto* node : branch) {
    if (node->type() == SetType::kPon) {
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
  const Hand& hand = state.hands[player];
  for (int i = 0; i < hand.meld_count; ++i) {
    if (hand.melds[i].type >= SetType::kPon) {
      if (hand.melds[i].start.getSuit() == Piece::Type::kBambooSuit) {
        bamboo_pon.at(hand.melds[i].start.getPieceNum() - 1) = true;
      }
      if (hand.melds[i].start.getSuit() == Piece::Type::kCharacterSuit) {
        char_pon.at(hand.melds[i].start.getPieceNum() - 1) = true;
      }
      if (hand.melds[i].start.getSuit() == Piece::Type::kPinSuit) {
        pin_pon.at(hand.melds[i].start.getPieceNum() - 1) = true;
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

REGISTER_YAKU({
    .id = "triplepon",
    .name = "Triple Pon",
    .type = Yaku::kOpen,
    .value = 2,
    .is_yaku_func = yaku::isTriplePon,
});
}  // namespace mahjong::yaku
