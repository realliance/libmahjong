#include <array>
#include <memory>

#include "statefunctions/router.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/settings.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> GameStart(std::unique_ptr<GameState> state) {
  for (int i = 0; i < 4; i++) {
    state->players[i].id = i;
    state->players[i].points = kStartingPoints;
    state->controllers[i]->GameStart(i);
  }
  state->g.seed(state->seed);
  state->nextState = StateFunctionType::kRoundStart;
  return state;
}
}  // namespace

REGISTER_ROUTE(GameStart, StateFunctionType::kGameStart);
}  // namespace mahjong
