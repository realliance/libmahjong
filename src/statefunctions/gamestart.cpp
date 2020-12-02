#include <array>
#include <random>

#include "gamestate.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"

auto Mahjong::GameStart(GameState& state) -> GameState& {
  for (int i = 0; i < 4; i++) {
    state.players.at(i).points = 25000;
    state.players.at(i).controller->GameStart(i);
  }
  state.g.seed(state.seed);
  state.nextState = RoundStart;
  return state;
}
