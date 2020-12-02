#include <array>
#include <vector>

#include "gamestate.h"
#include "hand.h"
#include "hands.h"
#include "piecetype.h"
#include "statefunctions.h"

auto Mahjong::Exhaust(GameState& state) -> GameState& {
  std::array<int, 4> winningPlayers = {};
  int totalWinners = 0;
  for (int i = 0; i < 4; i++) {
    // im taking the liberty to ignore the rule
    // that if your wait is a piece you have four
    // of you're then not in tenpai
    // shouldn't matter much
    // message Alice for complains
    if (state.hands.at(i).riichi || !isInTenpai13Pieces(state.hands.at(i).live).empty()) {
      winningPlayers.at(i) = 1;
      totalWinners++;
    }
  }
  state.counters++;
  if (winningPlayers.at(state.roundNum % 4) == 0) {
    state.roundNum++;
  }
  if (totalWinners < 4 && totalWinners > 0) {
    for (int i = 0; i < 4; i++) {
      if (winningPlayers.at(i) != 0) {
        state.scores.at(i) = 3000 / totalWinners;
      } else {
        switch (totalWinners) {
          case 1:
            state.scores.at(i) = -1000;
            break;
          case 2:
            state.scores.at(i) = -1500;
            break;
          case 3:
            state.scores.at(i) = -3000;
            break;
          default:
            break;
        }
      }
      if (state.hands.at(i).riichi) {
        state.scores.at(i) -= 1000;
      }
    }
  }
  state.nextState = RoundEnd;
  return state;
}
