#include "scoring/yakus/blessingofman.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isBlessingOfMan(const GameState& state, const Player& player,
                     const std::vector<const mahjong::Node*>& /*branch*/) {
  if (player.open) {
    return false;
  }
  if (state.turnNum > player.id) {
    return false;
  }
  if (state.lastCall >= 0) {
    return false;
  }
  if (player.hasRonned) {
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
