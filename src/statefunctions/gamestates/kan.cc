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
std::unique_ptr<GameState> Kan(std::unique_ptr<GameState> state) {
  AlertPlayers(*state, Event{
                           .type = Event::kKan,          // type
                           .player = state->lastCaller,  // player
                           .piece = static_cast<int16_t>(
                               state->pendingPiece.toUint8_t()),  // piece
                           .decision = false,                     // decision
                       });

  if (Hand& player = state->players[state->currentPlayer];
      player.riichi && player.discards_count == player.riichiPieceDiscard) {
    player.riichiPieceDiscard++;
  }

  state->lastCall = state->turnNum;
  state->concealedKan = false;
  state->turnNum++;
  state->currentPlayer = state->lastCaller;

  Hand& player = state->players[state->currentPlayer];
  player.open = true;
  if (RemovePieces(player, state->pendingPiece,
                   /*count=*/3) != 3) {
    std::cerr << "Not Enough Pieces to remove in kan" << '\n';
    state->nextState = StateFunctionType::kError;
    return state;
  }
  player.melds[player.meld_count++] = Meld{
      .type = SetType::kKan,
      .start = state->pendingPiece,
  };

  state->nextState = StateFunctionType::kKanDiscard;
  return state;
}
}  // namespace

REGISTER_ROUTE(Kan, StateFunctionType::kKan);
}  // namespace mahjong
