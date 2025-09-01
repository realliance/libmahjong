#include "scoring/yakus/blessingofman.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isBlessingOfMan(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).open) {
    return false;
  }
  if (state.turnNum > player) {
    return false;
  }
  if (state.lastCall >= 0) {
    return false;
  }
  if (state.hasRonned.at(player)) {
    return true;
  }
  return false;
}

REGISTER_YAKU({
    .id = "blessingofman",
    .name = "Blessing of Man",
    .type = Yaku::kYakuman,
    .value = 13,
    .is_yaku_func = yaku::isBlessingOfMan,
});
}  // namespace mahjong::yaku
