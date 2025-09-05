#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "analysis/util.h"
#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> Riichi(std::unique_ptr<GameState> state) {
  // TODO(#22): Ask the players if they want to riichi
  state->pendingPiece = getPossibleWaits(state->players[state->currentPlayer])
                            .begin()
                            ->second.front();

  AlertPlayers(*state,
               Event{
                   .type = Event::kRiichi,          // type
                   .player = state->currentPlayer,  // player
                   .piece = static_cast<int16_t>(
                       Piece(state->pendingPiece).toUint8_t()),  // piece
                   .decision = false,                            // decision
               });

  Hand& player = state->players[state->currentPlayer];
  player.riichiRound = state->turnNum;
  player.riichiPieceDiscard = player.discards_count;
  player.riichi = true;
  state->riichiSticks++;

  state->nextState = StateFunctionType::kDiscard;
  return state;
}
}  // namespace

REGISTER_ROUTE(Riichi, StateFunctionType::kRiichi);
}  // namespace mahjong
