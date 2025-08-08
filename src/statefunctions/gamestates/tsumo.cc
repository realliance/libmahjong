#include <array>
#include <cstdint>
#include <memory>

#include "analysis/scoring.h"
#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> Tsumo(std::unique_ptr<GameState> state) {
  AlertPlayers(*state, Event{
                           .type = Event::kTsumo,           // type
                           .player = state->currentPlayer,  // player
                           .piece = static_cast<int16_t>(
                               state->pendingPiece.toUint8_t()),  // piece
                           .decision = false,                     // decision
                       });
  const int basic_points =
      getBasicPoints(scoreHand(*state, state->currentPlayer));
  state->scores.at(state->currentPlayer) += state->riichiSticks * 1000;
  state->riichiSticks = 0;
  state->scores.at(state->currentPlayer) += state->counters * 300;

  for (int i = 0; i < 4; i++) {
    if (i == state->currentPlayer) {
      if (state->hands.at(state->currentPlayer).riichi) {
        state->scores.at(i) -= 1000;
      }
      continue;
    }
    int amount = 0;
    if (state->currentPlayer == state->roundNum % 4 ||
        i == state->roundNum % 4) {
      amount = 2 * basic_points;
    } else {
      amount = basic_points;
    }
    if ((amount % 100) != 0) {
      amount = amount + (100 - (amount % 100));
    }
    state->scores.at(i) -= amount;
    state->scores.at(i) -= state->counters * 100;
    if (state->hands.at(i).riichi) {
      state->scores.at(i) -= 1000;
    }
    state->scores.at(state->currentPlayer) += amount;
  }

  if (state->currentPlayer == state->roundNum % 4) {
    state->counters++;
  } else {
    state->roundNum++;
    state->counters = 0;
  }
  state->nextState = StateFunctionType::kRoundEnd;
  return state;
}
}  // namespace

REGISTER_ROUTE(Tsumo, StateFunctionType::kTsumo);
}  // namespace mahjong
