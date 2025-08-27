#include "scoring/yakus/threekans.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isThreeKans(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  int kans = 0;
  const Hand& hand = state.hands[player];
  for (int i = 0; i < hand.meld_count; ++i) {
    if (hand.melds[i].type >= SetType::kKan) {
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
