#include <array>
#include <cstdint>
#include <vector>

#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "piecetype.h"
#include "player.h"
#include "statefunctions.h"
#include "stateutilities.h"

auto mahjong::RoundEnd(GameState& state) -> GameState& {
  state.currentPlayer = -1;
  state.turnNum = -1;
  state.lastCall = -1;
  state.concealedKan = false;
  state.lastCaller = -1;
  state.pendingPiece = Piece(Piece::kError);
  state.hasRonned = {};

  state.hands = {};

  const int last_round = 3;
  if (state.roundNum > last_round && state.riichiSticks > 0) {
    std::vector<int> winners;
    int highscore = -100000;
    for (int i = 0; i < 4; i++) {
      if (state.players.at(i).points + state.scores.at(i) > highscore) {
        highscore = state.players.at(i).points + state.scores.at(i);
        winners.clear();
        winners.push_back(i);
      } else if (state.players.at(i).points + state.scores.at(i) == highscore) {
        winners.push_back(i);
      }
    }
    for (const auto& winner : winners) {
      state.scores.at(winner) += (state.riichiSticks * 1000) / winners.size();
    }
  }

  // TODO(alice): scoring
  for (int i = 0; i < 4; i++) {
    AlertPlayers(state,
                 Event{.type = Event::kPointDiff,
                       .player = i,
                       .piece = static_cast<int16_t>(state.scores.at(i) / 100),
                       .decision = false});
    state.players.at(i).points += state.scores.at(i);
  }
  state.scores = {};

  if (state.roundNum > last_round) {  // east only lmao
    state.nextState = GameEnd;
  } else {
    state.nextState = RoundStart;
  }
  return state;
}
