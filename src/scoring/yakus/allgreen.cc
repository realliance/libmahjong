#include "scoring/yakus/allgreen.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAllGreen(const GameState& /*state*/, const Hand& hand,
                const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : hand.live_range()) {
    if (!piece.isGreen()) {
      return false;
    }
  }
  for (const auto& meld : hand.melds_range()) {
    if (!meld.start.isGreen()) {
      return false;
    }
  }
  return true;
}

REGISTER_YAKU({
    .id = "allgreen",
    .name = "All Green",
    .type = Yaku::kYakuman,
    .value = 13,
    .is_yaku_func = yaku::isAllGreen,
});
}  // namespace mahjong::yaku
