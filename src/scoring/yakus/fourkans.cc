#include "scoring/yakus/fourkans.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isFourKans(const GameState&  /*state*/, const Player& player,
                const std::vector<const mahjong::Node*>& /*branch*/) {
  int kans = 0;
    for (const auto& meld : player.melds_range()) {
    if (meld.type >= SetType::kKan) {
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
