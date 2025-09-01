#include "scoring/yakus/twicepuredoublechi.h"

#include <cstddef>
#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "types/gamestate.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isTwicePureDoubleChi(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  int pairs = 0;
  for (size_t i = 0; i < branch.size(); i++) {
    if (branch.at(i)->type() != SetType::kChi) {
      continue;
    }
    for (size_t j = i + 1; j < branch.size(); j++) {
      if (branch.at(j)->type() == SetType::kChi &&
          branch.at(i)->start() == branch.at(j)->start()) {
        pairs++;
      }
    }
  }
  return pairs == 2;
}

REGISTER_YAKU({
    .id = "twicepuredoublechi",
    .name = "Twice Pure Double Chi",
    .type = Yaku::kClosed,
    .value = 3,
    .is_yaku_func = yaku::isTwicePureDoubleChi,
});
}  // namespace mahjong::yaku
