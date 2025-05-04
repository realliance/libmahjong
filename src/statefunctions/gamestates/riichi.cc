#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "analysis/hands.h"
#include "statefunctions/router.h"
#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/statefunction.h"

namespace mahjong {

std::unique_ptr<GameState> Riichi(std::unique_ptr<GameState> state) {
  // we should ask the players but
  //  deadlines prevent this
  //  -alice
  state->pendingPiece =
      getRiichiDiscard(state->hands.at(state->currentPlayer).live)[0];

  AlertPlayers(*state,
               Event{
                   .type = Event::kRiichi,          // type
                   .player = state->currentPlayer,  // player
                   .piece = static_cast<int16_t>(
                       Piece(state->pendingPiece).toUint8_t()),  // piece
                   .decision = false,                            // decision
               });

  state->hands.at(state->currentPlayer).riichiRound = state->turnNum;
  state->hands.at(state->currentPlayer).riichiPieceDiscard =
      state->hands.at(state->currentPlayer).discards.size();
  state->hands.at(state->currentPlayer).riichi = true;
  state->riichiSticks++;

  state->nextState = StateFunctionType::kDiscard;
  return state;
}

REGISTER_ROUTE(Riichi, StateFunctionType::kRiichi);
}  // namespace mahjong
