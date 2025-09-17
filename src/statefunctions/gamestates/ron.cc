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
std::unique_ptr<GameState> Ron(std::unique_ptr<GameState> state) {
  if (Hand& hand = state->hands[state->currentPlayer];
      hand.riichi && hand.discards_count == hand.riichiPieceDiscard) {
    state->riichiSticks--;
    hand.riichi = false;
  }

  state->hands[state->lastCaller]
      .live[state->hands[state->lastCaller].live_count++] = state->pendingPiece;

  std::array<int, 4> basic_points = {};
  for (Hand& hand : state->hands) {
    if (hand.hasRonned) {
      AlertPlayers(*state, Event{
                               .type = Event::kRon,  // type
                               .player = hand.id,    // player
                               .piece = static_cast<int16_t>(
                                   state->pendingPiece.toUint8_t()),  // piece
                               .decision = false,  // decision
                           });
      basic_points.at(hand.id) = getBasicPoints(scoreHand(*state, hand));
    }
    if (hand.riichi) {
      hand.score -= 1000;
    }
  }
  int payment = 0;
  for (Hand& hand : state->hands) {
    if (hand.hasRonned) {
      hand.score += 1000 * state->riichiSticks;
      state->riichiSticks = 0;
      hand.score += 300 * state->counters;
      payment += 300 * state->counters;
      if (hand.id == state->roundNum % 4) {
        int amount = 6 * basic_points.at(hand.id);
        if ((amount % 100) != 0) {
          amount = amount + (100 - (amount % 100));
        }
        hand.score += amount;
        payment += amount;
      } else {
        int amount = 4 * basic_points.at(hand.id);
        if ((amount % 100) != 0) {
          amount = amount + (100 - (amount % 100));
        }
        hand.score += amount;
        payment += amount;
      }
    }
  }

  state->hands[state->currentPlayer].score -= payment;

  if (state->hands[state->roundNum % 4].hasRonned) {
    state->counters++;
  } else {
    state->roundNum++;
    state->counters = 0;
  }

  bool allzeros = true;
  for (int i = 0; i < 4; i++) {
    if (state->hands[i].score != 0) {
      allzeros = false;
    }
  }
  if (allzeros) {
    throw "oof";
  }

  state->nextState = StateFunctionType::kRoundEnd;
  return state;
}
}  // namespace

REGISTER_ROUTE(Ron, StateFunctionType::kRon);
}  // namespace mahjong
