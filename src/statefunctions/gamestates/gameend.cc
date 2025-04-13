#include <array>
#include <utility>

#include "controllers/playercontroller.h"
#include "statefunctions/statefunctions.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/player.h"

namespace mahjong {

GameState&& GameEnd(GameState&& state) {
  for (auto& player : state.players) {
    player.controller->ReceiveEvent(kEndEvent);
    player.controller.reset();
  }
  return std::move(state);
}

}  // namespace mahjong
