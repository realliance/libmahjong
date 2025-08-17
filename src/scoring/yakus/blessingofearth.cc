#include "scoring/yakus/blessingofearth.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
bool isBlessingOfEarth(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).open) {
    return false;
  }
  if (state.turnNum > 3) {
    return false;
  }
  if (state.lastCall >= 0) {
    return false;
  }
  if (state.hasRonned.at(player)) {
    return false;
  }
  return true;
}

}  // namespace mahjong::yaku
