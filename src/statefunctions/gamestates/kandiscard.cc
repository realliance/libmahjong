#include <array>
#include <cstdint>
#include <utility>

#include "controllers/playercontroller.h"
#include "statefunctions/decisionfunction.h"
#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/player.h"

namespace mahjong {

GameState&& KanDiscard(GameState&& state) {
  std::array<bool, 4> need_decision = {false, false, false, false};
  for (int player = 0; player < 4; player++) {
    if (player == state.currentPlayer) {
      continue;
    }
    if (CanRon(state, player)) {
      need_decision.at(player) = true;
      state.players.at(state.currentPlayer)
          .controller->ReceiveEvent(Event{
              .type = Event::kRon,            // type
              .player = state.currentPlayer,  // player
              .piece = static_cast<int16_t>(
                  state.pendingPiece.toUint8_t()),  // piece
              .decision = true,                     // decision
          });
    }
  }

  bool have_ronned = false;
  for (int i = 0; i < 4; i++) {
    if (need_decision.at(i)) {
      Event temp_decision = GetValidDecisionOrThrow(state, i, /*inHand=*/false);
      if (temp_decision.type == Event::kRon) {
        state.hasRonned.at(i) = true;
        have_ronned = true;
      }
    }
  }

  if (have_ronned) {
    state.nextState = Ron;
  } else {
    state.nextState = Replacement;
  }

  return std::move(state);
}

}  // namespace mahjong
