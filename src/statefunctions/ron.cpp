#include <array>
#include <cstdint>
#include <vector>

#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "hands.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"

auto Mahjong::Ron(GameState& state) -> GameState& {
  state.hands.at(state.lastCaller).live.push_back(state.pendingPiece);
  state.hands.at(state.lastCaller).sort();

  std::array<int, 4> basicPoints = {};
  if (state.hands.at(state.currentPlayer).riichi &&
      state.hands.at(state.currentPlayer).discards.size() == state.hands.at(state.currentPlayer).riichiPieceDiscard) {
    state.riichiSticks--;
    state.hands.at(state.currentPlayer).riichi = false;
  }
  for (int player = 0; player < 4; player++) {
    if (state.hasRonned.at(player)) {
      AlertPlayers(state, Event{
                            Event::Ron,                                            // type
                            player,                                                // player
                            static_cast<int16_t>(state.pendingPiece.toUint8_t()),  // piece
                            false,                                                 // decision
                          });
      basicPoints.at(player) = getBasicPoints(scoreHand(state, player));
    }
    if (state.hands.at(player).riichi) {
      state.scores.at(player) -= 1000;
    }
  }
  int payment = 0;
  for (int i = 0; i < 4; i++) {
    int player = (state.roundNum + i) % 4;
    if (state.hasRonned.at(player)) {
      state.scores.at(player) += 1000 * state.riichiSticks;
      state.riichiSticks = 0;
      state.scores.at(player) += 300 * state.counters;
      payment += 300 * state.counters;
      if (i == state.roundNum % 4) {
        int amount = 6 * basicPoints.at(player);
        if ((amount % 100) != 0) {
          amount = amount + (100 - (amount % 100));
        }
        state.scores.at(player) += amount;
        payment += amount;
      } else {
        int amount = 4 * basicPoints.at(player);
        if ((amount % 100) != 0) {
          amount = amount + (100 - (amount % 100));
        }
        state.scores.at(player) += amount;
        payment += amount;
      }
    }
  }

  state.scores.at(state.currentPlayer) -= payment;

  if (state.hasRonned.at(state.roundNum % 4)) {
    state.counters++;
  } else {
    state.roundNum++;
    state.counters = 0;
  }

  bool allzeros = true;
  for (int i = 0; i < 4; i++) {
    if (state.scores.at(i) != 0) {
      allzeros = false;
    }
  }
  if (allzeros) {
    throw "oof";
  }

  state.nextState = RoundEnd;
  return state;
}
