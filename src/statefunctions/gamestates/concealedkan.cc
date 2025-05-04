#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "statefunctions/router.h"
#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/piecetype.h"
#include "types/statefunction.h"

namespace mahjong {

std::unique_ptr<GameState> ConcealedKan(std::unique_ptr<GameState> state) {
  AlertPlayers(*state, Event{
                           .type = Event::kConcealedKan,    // type
                           .player = state->currentPlayer,  // player
                           .piece = static_cast<int16_t>(
                               state->pendingPiece.toUint8_t()),  // piece
                           .decision = false,                     // decision
                       });
  if (RemovePieces(*state, state->currentPlayer, state->pendingPiece,
                   /*count=*/4) != 4) {
    std::cerr << "Not Enough pieces to remove in ConcealedKan" << '\n';
    state->nextState = Error;
    return state;
  }
  state->hands.at(state->currentPlayer)
      .melds.push_back({Meld::kConcealedKan, state->pendingPiece});
  state->concealedKan = true;
  state->nextState = KanDiscard;
  return state;
}

REGISTER_ROUTE(ConcealedKan, StateFunctionType::kConcealedKan);
}  // namespace mahjong
