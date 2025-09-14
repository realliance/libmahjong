#include "scoring/yakus/riichi.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isRiichi(const GameState& state, const Hand& hand,
              const std::vector<const mahjong::Node*>& /*branch*/) {
  return hand.riichi && !hand.open && (state.turnNum > 4 || state.lastCall < 0);
}

REGISTER_YAKU({
    .id = "riichi",
    .name = "Riichi",
    .type = Yaku::kClosed,
    .value = 1,
    .is_yaku_func = yaku::isRiichi,
});
}  // namespace mahjong::yaku
