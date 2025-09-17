#include "scoring/yakus/blessingofheaven.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isBlessingOfHeaven(const GameState& state, const Hand& hand,
                        const std::vector<const mahjong::Node*>& /*branch*/) {
  if (hand.open) {
    return false;
  }
  if (hand.id != 0) {
    return false;
  }
  if (state.hands[0].hasRonned) {
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
