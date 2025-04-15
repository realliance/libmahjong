#include <gtest/gtest.h>

#include "gamestate.h"
#include "statefunctions.h"
#include "types/settings.h"

namespace mahjong {

TEST(GameSetup, playerCount) {
  GameState state = GameStart(GameState{});
  EXPECT_EQ(state.players, kNumPlayers);
}

TEST(GameSetup, playerScore) {
  GameState state = GameStart(GameState{});
  for (const auto& score : state.scores) {
    EXPECT_EQ(score, kStartingPoints);
  }
}

TEST(RoundSetup, playerHands) {
  GameState state = RoundStart(GameStart(GameState{}));
  for(const auto& hand : state.hands){
    
  }
}

}  // namespace mahjong
