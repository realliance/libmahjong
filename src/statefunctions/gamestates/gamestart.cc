#include <array>
#include <memory>

#include "controllers/playercontroller.h"
#include "statefunctions/router.h"
#include "statefunctions/statefunctions.h"
#include "types/gamestate.h"
#include "types/settings.h"
#include "types/statefunction.h"

namespace mahjong {

std::unique_ptr<GameState> GameStart(std::unique_ptr<GameState> state) {
  for (int i = 0; i < 4; i++) {
    state->players.at(i).points = kStartingPoints;
    state->players.at(i).controller->GameStart(i);
  }
  state->g.seed(state->seed);
  state->nextState = StateFunctionType::kRoundStart;
  return state;
}

REGISTER_ROUTE(GameStart, StateFunctionType::kGameStart);
}  // namespace mahjong
