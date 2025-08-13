#include "scoring/yakus/littlethreedragons.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
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

}  // namespace mahjong::yakus
