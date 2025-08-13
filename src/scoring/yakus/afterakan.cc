#include "scoring/yakus/afterakan.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"
#include "types/statefunction.h"

namespace mahjong::yakus {
bool isAfterAKan(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.currentPlayer != player) {
    return false;
  }
  if (state.prevState == StateFunctionType::kReplacement) {
    return true;
  }
  return false;
}

}  // namespace mahjong::yakus
