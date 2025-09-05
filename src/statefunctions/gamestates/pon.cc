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
  state->players.at(state->lastCaller).open = true;

  AlertPlayers(*state, Event{
                           .type = Event::kPon,          // type
                           .player = state->lastCaller,  // player
                           .piece = static_cast<int16_t>(
                               state->pendingPiece.toUint8_t()),  // piece
                           .decision = false,                     // decision
                       });

  if (state->players.at(state->currentPlayer).riichi &&
      state->players.at(state->currentPlayer).discards_count ==
          state->players.at(state->currentPlayer).riichiPieceDiscard) {
    state->players.at(state->currentPlayer).riichiPieceDiscard++;
  }

  Hand& player = state->players.at(state->lastCaller);
  state->currentPlayer = state->lastCaller;
  state->lastCall = state->turnNum;
  state->concealedKan = false;
  state->turnNum++;

  if (RemovePieces(state->players[state->lastCaller], state->pendingPiece,
                   /*count=*/2) != 2) {
    std::cerr << "Not enough pieces to remove in Pon" << '\n';
    state->nextState = StateFunctionType::kError;
    return state;
  }
  player.melds[player.meld_count++] = Meld{
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
