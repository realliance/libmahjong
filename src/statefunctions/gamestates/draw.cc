#include <array>
#include <utility>
#include <vector>

#include "statefunctions/statefunctions.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/walls.h"

namespace mahjong {
GameState&& Draw(GameState&& state) {
  state.currentPlayer = (state.currentPlayer + 1) % 4;
  state.turnNum++;
  state.pendingPiece = state.walls.TakePiece();
  state.hands.at(state.currentPlayer).live.push_back(state.pendingPiece);
  state.hands.at(state.currentPlayer).sort();
  state.nextState = PlayerHand;
  return std::move(state);
}
}  // namespace mahjong
