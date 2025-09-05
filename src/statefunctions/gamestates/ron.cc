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
  Player& caller_player = state->players[state->lastCaller];
  caller_player.live[caller_player.live_count++] = state->pendingPiece;

  std::array<int, 4> basic_points = {};
  Player& curr_player = state->players[state->currentPlayer];
  if (curr_player.riichi &&
      curr_player.discards_count == curr_player.riichiPieceDiscard) {
    state->riichiSticks--;
    curr_player.riichi = false;
  }
  for (Player& player : state->players) {
    if (player.hasRonned) {
      AlertPlayers(*state, Event{
                               .type = Event::kRon,  // type
                               .player = player.id,     // player
                               .piece = static_cast<int16_t>(
                                   state->pendingPiece.toUint8_t()),  // piece
                               .decision = false,  // decision
                           });
      basic_points.at(player.id) = getBasicPoints(scorePlayer(*state, player));
    }
    if (player.riichi) {
      player.score -= 1000;
    }
  }
  int payment = 0;
  for (Player& player : state->players) {
    if (player.hasRonned) {
      player.score += 1000 * state->riichiSticks;
      state->riichiSticks = 0;
      player.score += 300 * state->counters;
      payment += 300 * state->counters;
      if (player.id == state->roundNum % 4) {
        int amount = 6 * basic_points.at(player.id);
        if ((amount % 100) != 0) {
          amount = amount + (100 - (amount % 100));
        }
        player.score += amount;
        payment += amount;
      } else {
        int amount = 4 * basic_points.at(player.id);
        if ((amount % 100) != 0) {
          amount = amount + (100 - (amount % 100));
        }
        player.score += amount;
        payment += amount;
      }
    }
  }

  state->players[state->currentPlayer].score -= payment;

  if (state->players.at(state->roundNum % 4).hasRonned) {
    state->counters++;
  } else {
    state->roundNum++;
    state->counters = 0;
  }

  bool allzeros = true;
  for (int i = 0; i < 4; i++) {
    if (state->players.at(i).score != 0) {
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
