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
  for (const Hand& player : state->players) {
    if (player.id == state->currentPlayer) {
      continue;
    }
    if (CanRon(*state, player)) {
      need_decision.at(player.id) = true;
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
  for (Hand& player : state->players) {
    if (need_decision.at(player.id)) {
      const Event temp_decision =
          GetValidDecisionOrThrow(*state, player, /*inPlayer=*/false);
      if (temp_decision.type == Event::kRon) {
        player.hasRonned = true;
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
