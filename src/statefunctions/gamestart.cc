#include <array>

#include "gamestate.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"

namespace mahjong {

GameState& GameStart(GameState& state) {
  for (int i = 0; i < 4; i++) {
    state.players.at(i).points = 25000;
    state.players.at(i).controller->GameStart(i);
  }
  state.g.seed(state.seed);
  state.nextState = RoundStart;
  return state;
}

}  // namespace mahjong
