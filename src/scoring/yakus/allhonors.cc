#include "scoring/yakus/allhonors.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAllHonors(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isHonor()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
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
