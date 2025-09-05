#include "scoring/yakus/allterminals.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAllTerminals(const GameState&  /*state*/, const Player& player,
                    const std::vector<const mahjong::Node*>& /*branch*/) {
    for (const auto& piece : player.live_range()) {
    if (!piece.isTerminal()) {
      return false;
    }
  }
  for (const auto& meld : player.melds_range()) {
    if (!meld.start.isTerminal()) {
      return false;
    }
  }
  return true;
}

REGISTER_YAKU({
    .id = "allterminals",
    .name = "All Terminals",
    .type = Yaku::kYakuman,
    .value = 1,
    .is_yaku_func = yaku::isAllTerminals,
});
}  // namespace mahjong::yaku
