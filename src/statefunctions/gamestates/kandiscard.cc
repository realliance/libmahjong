#include <array>
#include <cstdint>
#include <memory>

#include "statefunctions/decisionfunction.h"
#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> KanDiscard(std::unique_ptr<GameState> state) {
  std::array<bool, 4> need_decision = {false, false, false, false};
  for (const Hand& hand : state->hands) {
    if (hand.id == state->currentPlayer) {
      continue;
    }
    if (CanRon(*state, hand)) {
      need_decision.at(hand.id) = true;
      state->controllers.at(state->currentPlayer)
          ->ReceiveEvent(Event{
              .type = Event::kRon,             // type
              .player = state->currentPlayer,  // player
              .piece = static_cast<int16_t>(
                  state->pendingPiece.toUint8_t()),  // piece
              .decision = true,                      // decision
          });
    }
  }

  bool have_ronned = false;
  for (Hand& hand : state->hands) {
    if (need_decision.at(hand.id)) {
      const Event temp_decision =
          GetValidDecisionOrThrow(*state, hand, /*inHand=*/false);
      if (temp_decision.type == Event::kRon) {
        hand.hasRonned = true;
        have_ronned = true;
      }
    }
  }

  if (have_ronned) {
    state->nextState = StateFunctionType::kRon;
  } else {
    state->nextState = StateFunctionType::kReplacement;
  }

  return state;
}
}  // namespace

REGISTER_ROUTE(KanDiscard, StateFunctionType::kKanDiscard);
}  // namespace mahjong
