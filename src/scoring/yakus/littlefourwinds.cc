#include "scoring/yakus/littlefourwinds.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
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

}  // namespace mahjong::yaku
