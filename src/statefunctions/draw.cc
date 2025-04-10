#include <array>
#include <vector>

#include "gamestate.h"
#include "hand.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "walls.h"

namespace mahjong {
GameState& Draw(GameState& state) {
  state.currentPlayer = (state.currentPlayer + 1) % 4;
  state.turnNum++;
  state.pendingPiece = state.walls.TakePiece();
  state.hands.at(state.currentPlayer).live.push_back(state.pendingPiece);
  state.hands.at(state.currentPlayer).sort();
  state.nextState = PlayerHand;
  return state;
}
}  // namespace mahjong
