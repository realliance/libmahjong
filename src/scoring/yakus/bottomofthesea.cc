#include "scoring/yakus/bottomofthesea.h"

#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong::yakus {
bool isBottomOfTheSea(const GameState& state, int /*player*/,
                      const std::vector<const mahjong::Node*>& /*branch*/) {
  return state.walls.GetRemainingPieces() == 0;
}

}  // namespace mahjong::yakus
