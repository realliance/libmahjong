#include "scoring/yakus/allhonors.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yaku {
bool isAllHonors(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : state.hands.at(player).live) {
    if (!piece.isHonor()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (!meld.start.isHonor()) {
      return false;
    }
  }
  return true;
}

}  // namespace mahjong::yaku
