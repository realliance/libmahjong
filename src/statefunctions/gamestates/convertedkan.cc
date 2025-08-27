#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/statefunction.h"

namespace mahjong {
namespace {
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
  Hand& hand = state->hands.at(state->currentPlayer);
  for (int i = 0; i < hand.meld_count; ++i) {
    if (hand.melds[i].type == SetType::kPon &&
        hand.melds[i].start == state->pendingPiece) {
      hand.melds[i].type = SetType::kKan;
      state->nextState = StateFunctionType::kKanDiscard;
      return state;
    }
  }
  std::cerr << "Could Not find matching pon" << '\n';
  state->nextState = StateFunctionType::kError;
  return state;
}
}  // namespace

REGISTER_ROUTE(ConvertedKan, StateFunctionType::kConvertedKan);
}  // namespace mahjong
