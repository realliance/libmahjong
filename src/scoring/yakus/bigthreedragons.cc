#include "scoring/yakus/bigthreedragons.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isBigThreeDragons(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& branch) {
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
    if (node->type() == Node::kPair) {
      return false;
    }
    pons++;
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
