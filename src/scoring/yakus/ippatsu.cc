#include "scoring/yakus/ippatsu.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "scoring/yakus/doubleriichi.h"
#include "scoring/yakus/riichi.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isIppatsu(const GameState& state, const Hand& player,
               const std::vector<const mahjong::Node*>& /*branch*/) {
  return (isRiichi(state, player) || isDoubleRiichi(state, player)) &&
         (state.turnNum - player.riichiRound <= 4 &&
          state.lastCall < player.riichiRound);
}

REGISTER_YAKU({
    .id = "Ippastsu",
    .name = "Ippastsu",
    .type = Yaku::kClosed,
    .value = 1,
    .is_yaku_func = yaku::isIppatsu,
});
}  // namespace mahjong::yaku
