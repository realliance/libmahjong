#include <array>
#include <string>

#include "event.h"
#include "gamestate.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"

auto Mahjong::GameEnd(GameState& state) -> GameState& {
  for (auto& player : state.players) {
    player.controller->ReceiveEvent(END_EVENT);
#ifndef NO_PYBIND
    if (player.controller->Name() == "Player" || player.controller->Name() == "StubbornBot") {
      continue;
    }
#endif
    delete player.controller;
    player.controller = nullptr;
  }
  return state;
}
