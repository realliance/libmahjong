#include "scoring/yakus/threeconcealedpons.h"

#include <vector>

#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isThreeConcealedPons(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch) {
  int concealed_pons = 0;
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet) {
      concealed_pons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kConcealedKan) {
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
