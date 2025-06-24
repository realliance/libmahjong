#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

#include "statefunctions/router.h"
#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/piecetype.h"
#include "types/statefunction.h"

namespace mahjong {
std::unique_ptr<GameState> ConvertedKan(std::unique_ptr<GameState> state) {
  AlertPlayers(*state, Event{
                           .type = Event::kConvertedKan,    // type
                           .player = state->currentPlayer,  // player
                           .piece = static_cast<int16_t>(
                               state->pendingPiece.toUint8_t()),  // piece
                           .decision = false,                     // decision
                       });
  if (RemovePieces(*state, state->currentPlayer, state->pendingPiece,
                   /*count=*/1) != 1) {
    std::cerr << "Not Enough pieces to remove in ConvertedKan" << '\n';
    state->nextState = StateFunctionType::kError;
    return state;
  }
  state->concealedKan = false;
  for (auto& meld : state->hands.at(state->currentPlayer).melds) {
    if (meld.type == Meld::kPon && meld.start == state->pendingPiece) {
      meld.type = Meld::kKan;
      state->nextState = StateFunctionType::kKanDiscard;
      return state;
    }
  }
  std::cerr << "Could Not find matching pon" << '\n';
  state->nextState = StateFunctionType::kError;
  return state;
}

REGISTER_ROUTE(ConvertedKan, StateFunctionType::kConvertedKan);
}  // namespace mahjong
