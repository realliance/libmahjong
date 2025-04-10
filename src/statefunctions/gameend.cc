#include <array>
#include <string>

#include "event.h"
#include "gamestate.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"

namespace mahjong {

GameState& GameEnd(GameState& state) {
  for (auto& player : state.players) {
    player.controller->ReceiveEvent(kEndEvent);
    delete player.controller;
    player.controller = nullptr;
  }
  return state;
}

}  // namespace mahjong
