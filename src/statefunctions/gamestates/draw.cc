#include <array>
#include <memory>

#include "statefunctions/router.h"
#include "types/gamestate.h"
#include "types/statefunction.h"
#include "types/walls.h"

namespace mahjong {
namespace {
std::unique_ptr<GameState> Draw(std::unique_ptr<GameState> state) {
  state->currentPlayer = (state->currentPlayer + 1) % 4;
  state->turnNum++;
  state->pendingPiece = Walls::TakePiece(*state);
  Player& player =state->players[state->currentPlayer];
  player.live[player.live_count++] = state->pendingPiece;
  state->nextState = StateFunctionType::kPlayerPlayer;
  return state;
}
}  // namespace

REGISTER_ROUTE(Draw, StateFunctionType::kDraw);
}  // namespace mahjong
