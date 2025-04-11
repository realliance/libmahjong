#include <array>
#include <cstdint>
#include <utility>
#include <vector>

#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/walls.h"

namespace mahjong {
GameState&& Replacement(GameState&& state) {
  Piece draw = state.walls.TakeReplacementTile();
  state.hands.at(state.currentPlayer).live.push_back(draw);
  state.hands.at(state.currentPlayer).sort();
  state.pendingPiece = draw;

  AlertPlayers(state,
               Event{
                   .type = Event::kDora,  // type
                   .player = -1,          // player
                   .piece = static_cast<int16_t>(
                       state.walls.GetDoras().back().toUint8_t()),  // piece
                   .decision = false,                               // decision
               });

  state.nextState = PlayerHand;
  return std::move(state);
}

}  // namespace mahjong
