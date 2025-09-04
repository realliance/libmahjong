#include "scoring/yakus/littlefourwinds.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isLittleFourWinds(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& branch) {
  bool pair = false;
  int pons = 0;
  for (const auto* node : branch) {
    switch (node->start().toUint8_t()) {
      case Piece::Type::kEastWind:
      case Piece::Type::kSouthWind:
      case Piece::Type::kWestWind:
      case Piece::Type::kNorthWind:
        break;
      default:
        continue;
    }
    if (node->type() != SetType::kPair) {
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
  const Hand& hand = state.hands[player];
  for (const auto& meld : hand.melds_range()) {
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

REGISTER_YAKU({
    .id = "littlefourwinds",
    .name = "Little Four Winds",
    .type = Yaku::kYakuman,
    .value = 13,
    .is_yaku_func = yaku::isLittleFourWinds,
});
}  // namespace mahjong::yaku
