#include "scoring/yakus/allhonors.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAllHonors(const GameState& /*state*/, const Hand& hand,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : hand.live_range()) {
    if (!piece.isHonor()) {
      return false;
    }
  }
  for (const auto& meld : hand.melds_range()) {
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
