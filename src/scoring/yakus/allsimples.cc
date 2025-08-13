#include "scoring/yakus/allsimples.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isAllSimples(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& /*branch*/) {
  for (const auto& piece : state.hands.at(player).live) {
    if (piece.isHonor() || piece.isTerminal()) {
      return false;
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.start.isHonor() || meld.start.isTerminal()) {
      return false;
    }
  }

  return true;
}

}  // namespace mahjong::yakus
