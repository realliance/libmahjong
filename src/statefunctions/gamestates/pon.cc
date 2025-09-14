#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/meld.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> Pon(std::unique_ptr<GameState> state) {
  if (Hand& hand = state->hands[state->currentPlayer];
      hand.riichi && hand.discards_count == hand.riichiPieceDiscard) {
    hand.riichiPieceDiscard++;
  }

  state->currentPlayer = state->lastCaller;
  state->lastCall = state->turnNum;
  state->concealedKan = false;
  state->turnNum++;

  Hand& hand = state->hands.at(state->currentPlayer);
  hand.open = true;
  if (RemovePieces(hand, state->pendingPiece,
                   /*count=*/2) != 2) {
    std::cerr << "Not enough pieces to remove in Pon" << '\n';
    state->nextState = StateFunctionType::kError;
    return state;
  }
  hand.melds[hand.meld_count++] = Meld{
      .type = SetType::kPon,
      .start = state->pendingPiece,
  };

  AlertPlayers(*state, Event{
                           .type = Event::kPon,          // type
                           .player = state->lastCaller,  // player
                           .piece = static_cast<int16_t>(
                               state->pendingPiece.toUint8_t()),  // piece
                           .decision = false,                     // decision
                       });

  state->pendingPiece = AskForDiscard(*state);

  state->nextState = StateFunctionType::kDiscard;
  return state;
}
}  // namespace

REGISTER_ROUTE(Pon, StateFunctionType::kPon);
}  // namespace mahjong
