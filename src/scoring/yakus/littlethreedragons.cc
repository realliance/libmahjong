#include "scoring/yakus/littlethreedragons.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isLittleThreeDragons(const GameState& /*state*/, const Hand& player,
                          const std::vector<const mahjong::Node*>& branch) {
  bool pair = false;
  int pons = 0;
  for (const auto* node : branch) {
    switch (node->start().toUint8_t()) {
      case Piece::Type::kRedDragon:
      case Piece::Type::kGreenDragon:
      case Piece::Type::kWhiteDragon:
        break;
      default:
        continue;
    }
    if (node->type() == SetType::kPon) {
      pons++;
    }
    if (node->type() == SetType::kPair) {
      pair = true;
    }
  }
  if (!pair) {
    return false;
  }
  for (const auto& meld : player.melds_range()) {
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

REGISTER_YAKU({
    .id = "littlethreedragons",
    .name = "Little Three Dragons",
    .type = Yaku::kOpen,
    .value = 2,
    .is_yaku_func = yaku::isLittleThreeDragons,
});
}  // namespace mahjong::yaku
