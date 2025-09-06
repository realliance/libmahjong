#include <memory>

#include "statefunctions/router.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/settings.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> GameStart(std::unique_ptr<GameState> state) {
  int i = 0;
  for (Hand& player : state->players) {
    player.id = i++;
    player.points = kStartingPoints;
    state->controllers[player.id]->GameStart(player.id);
  }
  state->g.seed(state->seed);
  state->nextState = StateFunctionType::kRoundStart;
  return state;
}
}  // namespace

REGISTER_ROUTE(GameStart, StateFunctionType::kGameStart);
}  // namespace mahjong
