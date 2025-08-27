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
  for (const auto& piece : hand.live) {
    if (piece.isHonor() || piece.isTerminal()) {
      return false;
    }
  }
  for (int i = 0; i < hand.meld_count; ++i) {
    if (hand.melds[i].start.isHonor() || hand.melds[i].start.isTerminal()) {
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
