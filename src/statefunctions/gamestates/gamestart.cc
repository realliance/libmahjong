#include <array>
#include <utility>
#include <memory>

#include "controllers/playercontroller.h"
#include "statefunctions/statefunctions.h"
#include "types/gamestate.h"
#include "types/player.h"
#include "types/settings.h"

namespace mahjong {

GameState&& GameStart(GameState&& state) {
  for (int i = 0; i < 4; i++) {
    state.players.at(i).points = kStartingPoints;
    state.players.at(i).controller->GameStart(i);
  }
  state.g.seed(state.seed);
  state.nextState = RoundStart;
  return std::move(state);
}

}  // namespace mahjong
