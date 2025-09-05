#include "scoring/yakus/puredoublechi.h"

#include <cstddef>
#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "scoring/yakus/twicepuredoublechi.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/sets.h"
#include "types/yaku.h"

namespace mahjong::yaku {
bool isPureDoubleChi(const GameState& state, const Player& player,
                     const std::vector<const mahjong::Node*>& branch) {
  if (player.open) {
    return false;
  }
  for (size_t i = 0; i < branch.size(); i++) {
    if (branch.at(i)->type() != SetType::kChi) {
      continue;
    }
    for (size_t j = 0; j < branch.size(); j++) {
      if (i == j) {
        continue;
      }
      if (branch.at(i)->type() == branch[j]->type() &&
          branch.at(i)->start() == branch[j]->start()) {
        return !isTwicePureDoubleChi(state, player, branch);
      }
    }
  }
  return false;
}

REGISTER_YAKU({
    .id = "puredoublechi",
    .name = "PureDoubleChi",
    .type = Yaku::kClosed,
    .value = 1,
    .is_yaku_func = yaku::isPureDoubleChi,
});
}  // namespace mahjong::yaku
