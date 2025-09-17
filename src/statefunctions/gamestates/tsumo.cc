#include <array>
#include <cstdint>
#include <memory>

#include "scoring/scoring.h"
#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
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

  Hand& winning_hand = state->hands[state->currentPlayer];
  const int basic_points = getBasicPoints(scoreHand(*state, winning_hand));
  winning_hand.score += state->riichiSticks * 1000;
  state->riichiSticks = 0;
  winning_hand.score += state->counters * 300;

  for (Hand& hand : state->hands) {
    if (hand.id == winning_hand.id) {
      if (hand.riichi) {
        hand.score -= 1000;
      }
      continue;
    }
    int amount = 0;
    if (state->currentPlayer == state->roundNum % 4 ||
        hand.id == state->roundNum % 4) {
      amount = 2 * basic_points;
    } else {
      amount = basic_points;
    }
    if ((amount % 100) != 0) {
      amount = amount + (100 - (amount % 100));
    }
    hand.score -= amount;
    hand.score -= state->counters * 100;
    if (hand.riichi) {
      hand.score -= 1000;
    }
    winning_hand.score += amount;
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
