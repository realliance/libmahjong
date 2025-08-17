#include "scoring/yakus/robbingakan.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/statefunction.h"

namespace mahjong::yaku {
bool isRobbingAKan(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*branch*/) {
  if (!state.hasRonned.at(player)) {
    return false;
  }
  if (state.nextState == StateFunctionType::kKanDiscard) {
    return true;
  }
  return false;
}

}  // namespace mahjong::yaku
