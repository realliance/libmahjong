#include "scoring/yakus/blessingofearth.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isBlessingOfEarth(const GameState& state, const Hand& hand,
                       const std::vector<const mahjong::Node*>& /*branch*/) {
  if (hand.open) {
    return false;
  }
  if (state.turnNum > 3) {
    return false;
  }
  if (state.lastCall >= 0) {
    return false;
  }
  if (hand.hasRonned) {
    return false;
  }
  return true;
}

REGISTER_YAKU({
    .id = "blessingofearth",
    .name = "Blessing of Earth",
    .type = Yaku::kYakuman,
    .value = 13,
    .is_yaku_func = yaku::isBlessingOfEarth,
});
}  // namespace mahjong::yaku
