#include "scoring/yakus/bigthreedragons.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isBigThreeDragons(const GameState&  /*state*/, const Player& player,
                       const std::vector<const mahjong::Node*>& branch) {
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
    if (node->type() == SetType::kPair) {
      return false;
    }
    pons++;
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
  return pons == 3;
}

REGISTER_YAKU({
    .id = "bigthreedragons",
    .name = "Big Three Dragons",
    .type = Yaku::kYakuman,
    .value = 1,
    .is_yaku_func = yaku::isBigThreeDragons,
});
}  // namespace mahjong::yaku
