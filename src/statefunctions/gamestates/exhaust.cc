#include <array>
#include <vector>

#include "analysis/hands.h"
#include "statefunctions/statefunctions.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"

namespace mahjong {

GameState& Exhaust(GameState& state) {
  std::array<int, 4> winning_players = {};
  int total_winners = 0;
  for (int i = 0; i < 4; i++) {
    // im taking the liberty to ignore the rule
    // that if your wait is a piece you have four
    // of you're then not in tenpai
    // shouldn't matter much
    // message Alice for complains
    if (state.hands.at(i).riichi ||
        !isInTenpai13Pieces(state.hands.at(i).live).empty()) {
      winning_players.at(i) = 1;
      total_winners++;
    }
  }
  state.counters++;
  if (winning_players.at(state.roundNum % 4) == 0) {
    state.roundNum++;
  }
  if (total_winners < 4 && total_winners > 0) {
    for (int i = 0; i < 4; i++) {
      if (winning_players.at(i) != 0) {
        state.scores.at(i) = 3000 / total_winners;
      } else {
        switch (total_winners) {
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

}  // namespace mahjong
