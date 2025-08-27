#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

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
std::unique_ptr<GameState> Pon(std::unique_ptr<GameState> state) {
  state->hands.at(state->lastCaller).open = true;

  AlertPlayers(*state, Event{
                           .type = Event::kPon,          // type
                           .player = state->lastCaller,  // player
                           .piece = static_cast<int16_t>(
                               state->pendingPiece.toUint8_t()),  // piece
                           .decision = false,                     // decision
                       });

  if (state->hands.at(state->currentPlayer).riichi &&
      state->hands.at(state->currentPlayer).discards.size() ==
          state->hands.at(state->currentPlayer).riichiPieceDiscard) {
    state->hands.at(state->currentPlayer).riichiPieceDiscard++;
  }

  Hand& hand = state->hands.at(state->lastCaller);
  state->currentPlayer = state->lastCaller;
  hand.live.push_back(state->pendingPiece);
  state->lastCall = state->turnNum;
  state->concealedKan = false;
  state->turnNum++;

  if (RemovePieces(*state, state->lastCaller, state->pendingPiece,
                   /*count=*/3) != 3) {
    std::cerr << "Not enough pieces to remove in Pon" << '\n';
    state->nextState = StateFunctionType::kError;
    return state;
  }
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = state->pendingPiece,
  };

  state->pendingPiece = AskForDiscard(*state);

  state->nextState = StateFunctionType::kDiscard;
  return state;
}
}  // namespace

REGISTER_ROUTE(Pon, StateFunctionType::kPon);
}  // namespace mahjong
