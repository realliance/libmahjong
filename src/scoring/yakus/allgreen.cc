#include "scoring/yakus/allgreen.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAllGreen(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isGreen()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
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
