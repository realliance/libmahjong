#include <memory>

#include "controllers/playercontroller.h"
#include "statefunctions/router.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> GameEnd(std::unique_ptr<GameState> state) {
  for (auto& player : state->players) {
    player->ReceiveEvent(kEndEvent);
    player.reset();
  }
  return state;
}
}  // namespace

REGISTER_ROUTE(GameEnd, StateFunctionType::kGameEnd);
}  // namespace mahjong
