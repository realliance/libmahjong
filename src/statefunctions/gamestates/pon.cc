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
  if (Hand& player = state->players[state->currentPlayer];
      player.riichi && player.discards_count == player.riichiPieceDiscard) {
    player.riichiPieceDiscard++;
  }

  state->currentPlayer = state->lastCaller;
  state->lastCall = state->turnNum;
  state->concealedKan = false;
  state->turnNum++;

  Hand& player = state->players.at(state->currentPlayer);
  player.open = true;
  if (RemovePieces(player, state->pendingPiece,
                   /*count=*/2) != 2) {
    std::cerr << "Not enough pieces to remove in Pon" << '\n';
    state->nextState = StateFunctionType::kError;
    return state;
  }
  player.melds[player.meld_count++] = Meld{
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
