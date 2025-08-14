#include "scoring/yakus/allsimples.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAllSimples(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.isHonor() || piece.isTerminal()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
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
