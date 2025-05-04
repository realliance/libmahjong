#include <memory>

#include "controllers/playercontroller.h"
#include "statefunctions/router.h"
#include "statefunctions/statefunctions.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/player.h"
#include "types/statefunction.h"

namespace mahjong {

std::unique_ptr<GameState> GameEnd(std::unique_ptr<GameState> state) {
  for (auto& player : state->players) {
    player.controller->ReceiveEvent(kEndEvent);
    player.controller.reset();
  }
  return state;
}

REGISTER_ROUTE(GameEnd, StateFunctionType::kGameEnd);
}  // namespace mahjong
