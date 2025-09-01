#include "scoring/yakus/doubleriichi.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isDoubleRiichi(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& /*branch*/) {
  return state.hands.at(player).riichi && !state.hands.at(player).open &&
         (state.turnNum < 4 && state.lastCall < 0);
}

REGISTER_YAKU({
    .id = "doubleriichi",
    .name = "Double Riichi",
    .type = Yaku::kClosed,
    .value = 2,
    .is_yaku_func = yaku::isDoubleRiichi,
});
}  // namespace mahjong::yaku
