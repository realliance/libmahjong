#include <array>
#include <memory>
#include <vector>

#include "analysis/util.h"
#include "statefunctions/router.h"
#include "types/gamestate.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> Exhaust(std::unique_ptr<GameState> state) {
  std::array<int, 4> winning_players = {};
  int total_winners = 0;
  for (int i = 0; i < 4; i++) {
    // TODO(#21): Implement no tenpai if you have all pieces of your wait
    if (state->hands.at(i).riichi ||
        !isInTenpai13Pieces(state->hands.at(i).live, /*allWaits=*/false)
             .empty()) {
      winning_players.at(i) = 1;
      total_winners++;
    }
  }
  state->counters++;
  if (winning_players.at(state->roundNum % 4) == 0) {
    state->roundNum++;
  }
  if (total_winners < 4 && total_winners > 0) {
    for (int i = 0; i < 4; i++) {
      if (winning_players.at(i) != 0) {
        state->scores.at(i) = 3000 / total_winners;
      } else {
        switch (total_winners) {
          case 1:
            state->scores.at(i) = -1000;
            break;
          case 2:
            state->scores.at(i) = -1500;
            break;
          case 3:
            state->scores.at(i) = -3000;
            break;
          default:
            break;
        }
      }
      if (state->hands.at(i).riichi) {
        state->scores.at(i) -= 1000;
      }
    }
  }
  state->nextState = StateFunctionType::kRoundEnd;
  return state;
}
}  // namespace

REGISTER_ROUTE(Exhaust, StateFunctionType::kExhaust);
}  // namespace mahjong
