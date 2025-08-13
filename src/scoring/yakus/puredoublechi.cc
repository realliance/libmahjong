#include "scoring/yakus/puredoublechi.h"

#include <cstddef>
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isPureDoubleChi(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  for (size_t i = 0; i < branch.size(); i++) {
    if (branch.at(i)->type() != Node::kChiSet) {
      continue;
    }
    for (size_t j = 0; j < branch.size(); j++) {
      if (i == j) {
        continue;
      }
      if (branch.at(i)->type() == branch[j]->type() &&
          branch.at(i)->start() == branch[j]->start()) {
        return true;
      }
    }
  }
  return false;
}

}  // namespace mahjong::yakus
