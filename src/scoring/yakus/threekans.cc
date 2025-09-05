#include "scoring/yakus/threekans.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isThreeKans(const GameState& /* state */, const Player& player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  int kans = 0;
    for (const auto& meld : player.melds_range()) {
    if (meld.type >= SetType::kKan) {
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
