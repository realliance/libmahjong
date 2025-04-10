#include <array>
#include <cstdint>

#include "decisionfunction.h"
#include "event.h"
#include "gamestate.h"
#include "piecetype.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"
#include "stateutilities.h"

namespace mahjong {

GameState& KanDiscard(GameState& state) {
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

  return state;
}

}  // namespace mahjong
