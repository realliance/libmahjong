#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> RoundEnd(std::unique_ptr<GameState> state) {
  state->currentPlayer = -1;
  state->turnNum = -1;
  state->lastCall = -1;
  state->concealedKan = false;
  state->lastCaller = -1;
  state->pendingPiece = Piece(Piece::kError);
  state->hasRonned = {};

  state->hands = {};

  const int last_round = 3;
  if (state->roundNum > last_round && state->riichiSticks > 0) {
    std::vector<int> winners;
    int highscore = -100000;
    for (int i = 0; i < 4; i++) {
      if (state->players.at(i).points + state->scores.at(i) > highscore) {
        highscore = state->players.at(i).points + state->scores.at(i);
        winners.clear();
        winners.push_back(i);
      } else if (state->players.at(i).points + state->scores.at(i) ==
                 highscore) {
        winners.push_back(i);
      }
    }
    for (const auto& winner : winners) {
      state->scores.at(winner) += (state->riichiSticks * 1000) / winners.size();
    }
  }

  // TODO(#17): Scoring
  for (int i = 0; i < 4; i++) {
    AlertPlayers(*state,
                 Event{.type = Event::kPointDiff,
                       .player = i,
                       .piece = static_cast<int16_t>(state->scores.at(i) / 100),
                       .decision = false});
    state->players.at(i).points += state->scores.at(i);
  }
  state->scores = {};

  // TODO (#16): East wind only for now
  if (state->roundNum > last_round) { 
    state->nextState = StateFunctionType::kGameEnd;
  } else {
    state->nextState = StateFunctionType::kRoundStart;
  }
  return state;
}
}  // namespace

REGISTER_ROUTE(RoundEnd, StateFunctionType::kRoundEnd);
}  // namespace mahjong
