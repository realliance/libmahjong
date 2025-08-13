#include "scoring/yakus/fullyconcealedhand.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isFullyConcealedHand(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& /*branch*/) {
  return state.currentPlayer == player && !state.hands.at(player).open &&
         state.walls.GetRemainingPieces() > 0;
}

}  // namespace mahjong::yakus
