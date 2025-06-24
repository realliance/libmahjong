#include <array>
#include <memory>
#include <vector>

#include "statefunctions/router.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/statefunction.h"
#include "types/walls.h"

namespace mahjong {
namespace {
std::unique_ptr<GameState> Draw(std::unique_ptr<GameState> state) {
  state->currentPlayer = (state->currentPlayer + 1) % 4;
  state->turnNum++;
  state->pendingPiece = state->walls.TakePiece();
  state->hands.at(state->currentPlayer).live.push_back(state->pendingPiece);
  state->hands.at(state->currentPlayer).sort();
  state->nextState = StateFunctionType::kPlayerHand;
  return state;
}
}  // namespace

REGISTER_ROUTE(Draw, StateFunctionType::kDraw);
}  // namespace mahjong
