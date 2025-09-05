#include "scoring/yakus/bigfourwinds.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isBigFourWinds(const GameState&  /*state*/, const Player& player,
                    const std::vector<const mahjong::Node*>& branch) {
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
    if (node->type() == SetType::kPair) {
      return false;
    }
    pons++;
  }
    for (const auto& meld : player.melds_range()) {
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

REGISTER_YAKU({
    .id = "bigfourwinds",
    .name = "Big Four Winds",
    .type = Yaku::kYakuman,
    .value = 13,
    .is_yaku_func = yaku::isBigFourWinds,
});
}  // namespace mahjong::yaku
