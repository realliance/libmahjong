#include "scoring/yakus/allpons.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isAllPons(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& branch) {
  int pons = 0;
  for (const auto* node : branch) {
    if (node->type() == SetType::kPon) {
      pons++;
    }
  }
  const Hand& hand = state.hands[player];
  for (int i = 0; i < hand.meld_count; ++i) {
    if (hand.melds[i].type == SetType::kKan ||
        hand.melds[i].type == SetType::kPon ||
        hand.melds[i].type == SetType::kConcealedKan) {
      pons++;
    }
  }
  return pons == 4;
}

REGISTER_YAKU({
    .id = "allpons",
    .name = "All Pons",
    .type = Yaku::kOpen,
    .value = 2,
    .is_yaku_func = yaku::isAllPons,
});
}  // namespace mahjong::yaku
