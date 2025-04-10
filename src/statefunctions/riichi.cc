#include <array>
#include <cstdint>
#include <vector>

#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "hands.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"

namespace mahjong {

GameState& Riichi(GameState& state) {
  // we should ask the players but
  //  deadlines prevent this
  //  -alice
  state.pendingPiece =
      getRiichiDiscard(state.hands.at(state.currentPlayer).live)[0];

  AlertPlayers(state, Event{
                          .type = Event::kRiichi,         // type
                          .player = state.currentPlayer,  // player
                          .piece = static_cast<int16_t>(
                              Piece(state.pendingPiece).toUint8_t()),  // piece
                          .decision = false,  // decision
                      });

  state.hands.at(state.currentPlayer).riichiRound = state.turnNum;
  state.hands.at(state.currentPlayer).riichiPieceDiscard =
      state.hands.at(state.currentPlayer).discards.size();
  state.hands.at(state.currentPlayer).riichi = true;
  state.riichiSticks++;

  state.nextState = Discard;
  return state;
}
}  // namespace mahjong
