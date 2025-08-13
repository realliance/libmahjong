#include "scoring/yakus/triplepon.h"

#include <array>
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
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

}  // namespace mahjong::yaku
