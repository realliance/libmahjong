#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> Kan(std::unique_ptr<GameState> state) {
  AlertPlayers(*state, Event{
                           .type = Event::kKan,          // type
                           .player = state->lastCaller,  // player
                           .piece = static_cast<int16_t>(
                               state->pendingPiece.toUint8_t()),  // piece
                           .decision = false,                     // decision
                       });

  if (state->hands.at(state->currentPlayer).riichi &&
      state->hands.at(state->currentPlayer).discards_count ==
          state->hands.at(state->currentPlayer).riichiPieceDiscard) {
    state->hands.at(state->currentPlayer).riichiPieceDiscard++;
  }

  Hand& hand = state->hands.at(state->lastCaller);
  hand.open = true;
  state->currentPlayer = state->lastCaller;
  state->lastCall = state->turnNum;
  state->concealedKan = false;
  state->turnNum++;

  if (RemovePieces(*state, state->lastCaller, state->pendingPiece,
                   /*count=*/3) != 3) {
    std::cerr << "Not Enough Pieces to remove in kan" << '\n';
    state->nextState = StateFunctionType::kError;
    return state;
  }
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kKan,
      .start = state->pendingPiece,
  };

  state->nextState = StateFunctionType::kKanDiscard;
  return state;
}
}  // namespace

REGISTER_ROUTE(Kan, StateFunctionType::kKan);
}  // namespace mahjong
