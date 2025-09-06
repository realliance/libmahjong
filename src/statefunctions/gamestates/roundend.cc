#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
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
  for (auto& player : state->players) {
    player.hasRonned = false;
  }

  state->controllers = {};

  const int last_round = 3;
  if (state->roundNum > last_round && state->riichiSticks > 0) {
    std::vector<int> winners;
    int highscore = -100000;
    for (const Hand& player : state->players) {
      if (player.points + player.score > highscore) {
        highscore = player.points + player.score;
        winners.clear();
        winners.push_back(player.id);
      } else if (player.points + player.score == highscore) {
        winners.push_back(player.id);
      }
    }
    for (const auto& winner : winners) {
      state->players.at(winner).score +=
          (state->riichiSticks * 1000) / winners.size();
    }
  }

  // TODO(#17): Scoring
  for (Hand& player : state->players) {
    AlertPlayers(*state,
                 Event{.type = Event::kPointDiff,
                       .player = player.id,
                       .piece = static_cast<int16_t>(player.score / 100),
                       .decision = false});
    player.points += player.score;
    player.score = 0;
  }

  // TODO (#14): for now naively increment the round number
  state->roundNum++;

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
