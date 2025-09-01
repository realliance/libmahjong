#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
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
    state->nextState = StateFunctionType::kError;
    return state;
  }
  state->hands.at(state->currentPlayer)
      .melds.push_back({SetType::kConcealedKan, state->pendingPiece});
  state->concealedKan = true;
  state->nextState = StateFunctionType::kKanDiscard;
  return state;
}
}  // namespace

REGISTER_ROUTE(ConcealedKan, StateFunctionType::kConcealedKan);
}  // namespace mahjong
