#include <array>
#include <cstdint>
#include <vector>

#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"
#include "walls.h"

auto mahjong::Replacement(GameState& state) -> GameState& {
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
  return state;
}
