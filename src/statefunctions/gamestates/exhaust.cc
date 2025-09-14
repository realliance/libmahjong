#include <array>
#include <memory>

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
    if (state->hands[i].riichi || !getWaits(state->hands[i]).empty()) {
      winning_players[i] = 1;
      total_winners++;
    }
  }
  state->counters++;
  if (winning_players.at(state->roundNum % 4) == 0) {
    state->roundNum++;
  }
  if (total_winners < 4 && total_winners > 0) {
    for (int i = 0; i < 4; i++) {
      if (winning_players[i] != 0) {
        state->hands[i].score = 3000 / total_winners;
      } else {
        switch (total_winners) {
          case 1:
            state->hands[i].score = -1000;
            break;
          case 2:
            state->hands[i].score = -1500;
            break;
          case 3:
            state->hands[i].score = -3000;
            break;
          default:
            break;
        }
      }
      if (state->hands[i].riichi) {
        state->hands[i].score -= 1000;
      }
    }
  }
  state->nextState = StateFunctionType::kRoundEnd;
  return state;
}
}  // namespace

REGISTER_ROUTE(Exhaust, StateFunctionType::kExhaust);
}  // namespace mahjong
