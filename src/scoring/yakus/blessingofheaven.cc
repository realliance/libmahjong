#include "scoring/yakus/blessingofheaven.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isBlessingOfHeaven(const GameState& state, int player,
                        const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).open) {
    return false;
  }
  if (player != 0) {
    return false;
  }
  if (state.hasRonned[0]) {
    return false;
  }
  if (state.turnNum != 0) {
    return false;
  }
  return true;
}

REGISTER_YAKU({
    .id = "blessingofheaven",
    .name = "Blessing of Heaven",
    .type = Yaku::kYakuman,
    .value = 13,
    .is_yaku_func = yaku::isBlessingOfHeaven,
});
}  // namespace mahjong::yaku
