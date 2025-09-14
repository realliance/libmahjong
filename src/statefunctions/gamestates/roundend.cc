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
  for (auto& hand : state->hands) {
    hand.hasRonned = false;
  }

  state->controllers = {};

  const int last_round = 3;
  if (state->roundNum > last_round && state->riichiSticks > 0) {
    std::vector<int> winners;
    int highscore = -100000;
    for (const Hand& hand : state->hands) {
      if (hand.points + hand.score > highscore) {
        highscore = hand.points + hand.score;
        winners.clear();
        winners.push_back(hand.id);
      } else if (hand.points + hand.score == highscore) {
        winners.push_back(hand.id);
      }
    }
    for (const auto& winner : winners) {
      state->hands.at(winner).score +=
          (state->riichiSticks * 1000) / winners.size();
    }
  }

  // TODO(#17): Scoring
  for (Hand& hand : state->hands) {
    AlertPlayers(*state, Event{.type = Event::kPointDiff,
                               .player = hand.id,
                               .piece = static_cast<int16_t>(hand.score / 100),
                               .decision = false});
    hand.points += hand.score;
    hand.score = 0;
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
