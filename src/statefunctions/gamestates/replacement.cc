#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "statefunctions/router.h"
#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/statefunction.h"
#include "types/walls.h"

namespace mahjong {
std::unique_ptr<GameState> Replacement(std::unique_ptr<GameState> state) {
  const Piece draw = state->walls.TakeReplacementTile();
  state->hands.at(state->currentPlayer).live.push_back(draw);
  state->hands.at(state->currentPlayer).sort();
  state->pendingPiece = draw;

  AlertPlayers(*state,
               Event{
                   .type = Event::kDora,  // type
                   .player = -1,          // player
                   .piece = static_cast<int16_t>(
                       state->walls.GetDoras().back().toUint8_t()),  // piece
                   .decision = false,                                // decision
               });

  state->nextState = StateFunctionType::kPlayerHand;
  return state;
}

REGISTER_ROUTE(Replacement, StateFunctionType::kReplacement);
}  // namespace mahjong
