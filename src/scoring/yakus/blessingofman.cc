#include "scoring/yakus/blessingofman.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
bool isBlessingOfMan(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& /*branch*/) {
  if (state.hands.at(player).open) {
    return false;
  }
  if (state.turnNum > player) {
    return false;
  }
  if (state.lastCall >= 0) {
    return false;
  }
  if (state.hasRonned.at(player)) {
    return true;
  }
  return false;
}

}  // namespace mahjong::yaku
