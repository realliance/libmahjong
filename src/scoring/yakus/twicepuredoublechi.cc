#include "scoring/yakus/twicepuredoublechi.h"

#include <cstddef>
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
bool isTwicePureDoubleChi(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch) {
  if (state.hands.at(player).open) {
    return false;
  }
  int pairs = 0;
  for (size_t i = 0; i < branch.size(); i++) {
    if (branch.at(i)->type() != Node::kChiSet) {
      continue;
    }
    for (size_t j = i + 1; j < branch.size(); j++) {
      if (branch.at(j)->type() == Node::kChiSet &&
          branch.at(i)->start() == branch.at(j)->start()) {
        pairs++;
      }
    }
  }
  return pairs == 2;
}

}  // namespace mahjong::yaku
