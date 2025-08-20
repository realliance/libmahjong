#include "scoring/yakus/threekans.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isThreeKans(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  int kans = 0;
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::kKan) {
      kans++;
    }
  }
  return kans >= 3;
}

REGISTER_YAKU({
    .id = "threekans",
    .name = "Three Kans",
    .type = Yaku::kOpen,
    .value = 2,
    .is_yaku_func = yaku::isThreeKans,
});
}  // namespace mahjong::yaku
