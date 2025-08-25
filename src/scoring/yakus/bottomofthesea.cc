#include "scoring/yakus/bottomofthesea.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/walls.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isBottomOfTheSea(const GameState& state, int /*player*/,
                      const std::vector<const mahjong::Node*>& /*branch*/) {
  return Walls::GetRemainingPieces(state) == 0;
}

REGISTER_YAKU({
    .id = "bottomofthesea",
    .name = "Bottom of the Sea",
    .type = Yaku::kOpen,
    .value = 1,
    .is_yaku_func = yaku::isBottomOfTheSea,
});
}  // namespace mahjong::yaku
