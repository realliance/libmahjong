#include "scoring/yakus/robbingakan.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/statefunction.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isRobbingAKan(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*branch*/) {
  if (!state.hasRonned.at(player)) {
    return false;
  }
  if (state.nextState == StateFunctionType::kKanDiscard) {
    return true;
  }
  return false;
}

REGISTER_YAKU({
    .id = "robbingakan",
    .name = "Robbing a Kan",
    .type = Yaku::kOpen,
    .value = 1,
    .is_yaku_func = yaku::isRobbingAKan,
});
}  // namespace mahjong::yaku
