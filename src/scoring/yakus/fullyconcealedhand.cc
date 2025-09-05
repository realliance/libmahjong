#include "scoring/yakus/fullyconcealedhand.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/walls.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isFullyConcealedHand(
    const GameState& state, const Player& player,
    const std::vector<const mahjong::Node*>& /*branch*/) {
  return state.currentPlayer == player.id && !player.open &&
         Walls::GetRemainingPieces(state) > 0;
}

REGISTER_YAKU({
    .id = "fullyconcealedhand",
    .name = "Fully Concealed Player",
    .type = Yaku::kClosed,
    .value = 1,
    .is_yaku_func = yaku::isFullyConcealedHand,
});
}  // namespace mahjong::yaku
