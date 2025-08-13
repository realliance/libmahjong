#include "scoring/yakus/blessingofheaven.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isBlessingOfHeaven(const GameState& state, int player,
                        const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).open) {
    return false;
  }
  if (player != 0) {
    return false;
  }
  if (state.hasRonned[0]) {
    return false;
  }
  if (state.turnNum != 0) {
    return false;
  }
  return true;
}

}  // namespace mahjong::yakus
