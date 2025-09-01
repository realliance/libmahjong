#include "scoring/yakus/fourconcealedpon.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isFourConcealedPon(const GameState& state, int player,
                        const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  int concealed_pons = 0;
  for (const auto* node : branch) {
    if (node->type() == SetType::kPon) {
      concealed_pons++;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == SetType::kConcealedKan) {
      concealed_pons++;
    }
  }
  return concealed_pons == 4;
}

REGISTER_YAKU({
    .id = "fourconcealedpon",
    .name = "Four Concealed Pon",
    .type = Yaku::kYakuman,
    .value = 13,
    .is_yaku_func = yaku::isFourConcealedPon,
});
}  // namespace mahjong::yaku
