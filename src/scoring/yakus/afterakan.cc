#include "scoring/yakus/afterakan.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/statefunction.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAfterAKan(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.currentPlayer != player) {
    return false;
  }
  if (state.prevState == StateFunctionType::kReplacement) {
    return true;
  }
  return false;
}

REGISTER_YAKU({
    .id = "afterakan",
    .name = "After a Kan",
    .type = Yaku::kOpen,
    .value = 1,
    .is_yaku_func = yaku::isAfterAKan,
});
}  // namespace mahjong::yaku
