#include "scoring/yakus/threeconcealedpons.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isThreeConcealedPons(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch) {
  int concealed_pons = 0;
  for (const auto* node : branch) {
    if (node->type() == SetType::kPon) {
      concealed_pons++;
    }
  }
  const Hand& hand = state.hands[player];
  for (const auto& meld : hand.melds_range()) {
    if (meld.type == SetType::kConcealedKan) {
      concealed_pons++;
    }
  }
  return concealed_pons >= 3;
}

REGISTER_YAKU({
    .id = "threeconcealedpons",
    .name = "Three Concealed Pons",
    .type = Yaku::kOpen,
    .value = 2,
    .is_yaku_func = yaku::isThreeConcealedPons,
});
}  // namespace mahjong::yaku
