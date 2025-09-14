#include "scoring/yakus/allterminalsandhonors.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAllTerminalsAndHonors(
    const GameState& /*state*/, const Hand& hand,
    const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : hand.live_range()) {
    if (!piece.isHonor() && !piece.isTerminal()) {
      return false;
    }
  }
  for (const auto& meld : hand.melds_range()) {
    if (!meld.start.isHonor() && !meld.start.isTerminal()) {
      return false;
    }
  }
  return true;
}

REGISTER_YAKU({
    .id = "allterminalsandhonors",
    .name = "All Terminals and Honors",
    .type = Yaku::kOpen,
    .value = 2,
    .is_yaku_func = yaku::isAllTerminalsAndHonors,
});
}  // namespace mahjong::yaku
