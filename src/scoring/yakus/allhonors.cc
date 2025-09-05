#include "scoring/yakus/allhonors.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAllHonors(const GameState&  /*state*/, const Player& player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
    for (const auto& piece : player.live_range()) {
    if (!piece.isHonor()) {
      return false;
    }
  }
  for (const auto& meld : player.melds_range()) {
    if (!meld.start.isHonor()) {
      return false;
    }
  }
  return true;
}

REGISTER_YAKU({
    .id = "allhonors",
    .name = "All Honors",
    .type = Yaku::kYakuman,
    .value = 13,
    .is_yaku_func = yaku::isAllHonors,
});
}  // namespace mahjong::yaku
