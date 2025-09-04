#include "scoring/yakus/allsimples.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAllSimples(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& /*branch*/) {
  const Hand& hand = state.hands[player];
  for (const auto& piece : hand.live_range()) {
    if (piece.isHonor() || piece.isTerminal()) {
      return false;
    }
  }
  for (const auto& meld : hand.melds_range()) {
    if (meld.start.isHonor() || meld.start.isTerminal()) {
      return false;
    }
  }

  return true;
}

REGISTER_YAKU({
    .id = "allsimples",
    .name = "All Simples",
    .type = Yaku::kOpen,
    .value = 1,
    .is_yaku_func = yaku::isAllSimples,
});
}  // namespace mahjong::yaku
