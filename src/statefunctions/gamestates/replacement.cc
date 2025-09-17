#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/statefunction.h"
#include "types/walls.h"

namespace mahjong {
namespace {
std::unique_ptr<GameState> Replacement(std::unique_ptr<GameState> state) {
  const Piece draw = Walls::TakeReplacementTile(*state);
  Hand& hand = state->hands[state->currentPlayer];
  hand.live[hand.live_count++] = draw;
  state->pendingPiece = draw;

  AlertPlayers(*state,
               Event{
                   .type = Event::kDora,  // type
                   .player = -1,          // player
                   .piece = static_cast<int16_t>(
                       Walls::GetDoras(*state).back().toUint8_t()),  // piece
                   .decision = false,                                // decision
               });

  state->nextState = StateFunctionType::kPlayerHand;
  return state;
}
}  // namespace

REGISTER_ROUTE(Replacement, StateFunctionType::kReplacement);
}  // namespace mahjong
