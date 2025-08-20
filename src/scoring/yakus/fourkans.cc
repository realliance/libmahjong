#include "scoring/yakus/fourkans.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isFourKans(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*branch*/) {
  int kans = 0;
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type >= Meld::kKan) {
      kans++;
    }
  }
  return kans == 4;
}

REGISTER_YAKU({
    .id = "fourkans",
    .name = "Four Kans",
    .type = Yaku::kYakuman,
    .value = 1,
    .is_yaku_func = yaku::isFourKans,
});
}  // namespace mahjong::yaku
