#include <array>
#include <memory>
#include <utility>

#include "controllers/playercontroller.h"
#include "statefunctions/statefunctions.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/player.h"

namespace mahjong {

std::unique_ptr<GameState> GameEnd(std::unique_ptr<GameState> state) {
  for (auto& player : state->players) {
    player.controller->ReceiveEvent(kEndEvent);
    player.controller.reset();
  }
  return std::move(state);
}

}  // namespace mahjong
