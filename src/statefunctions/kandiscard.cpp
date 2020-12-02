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

auto Mahjong::KanDiscard(GameState& state) -> GameState& {
  std::array<bool, 4> needDecision = {false, false, false, false};
  for (int player = 0; player < 4; player++) {
    if (player == state.currentPlayer) {
      continue;
    }
    if (CanRon(state, player)) {
      needDecision.at(player) = true;
      state.players.at(state.currentPlayer).controller->ReceiveEvent(Event{
        Event::Ron,                                            // type
        state.currentPlayer,                                   // player
        static_cast<int16_t>(state.pendingPiece.toUint8_t()),  // piece
        true,                                                  // decision
      });
    }
  }

  bool haveRonned = false;
  for (int i = 0; i < 4; i++) {
    if (needDecision.at(i)) {
      Event tempDecision = GetValidDecisionOrThrow(state, i, /*inHand=*/false);
      if (tempDecision.type == Event::Ron) {
        state.hasRonned.at(i) = true;
        haveRonned = true;
      }
    }
  }

  if (haveRonned) {
    state.nextState = Ron;
  } else {
    state.nextState = Replacement;
  }

  return state;
}
