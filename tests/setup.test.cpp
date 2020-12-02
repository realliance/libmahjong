#include <gtest/gtest.h>
#include "statefunctions.h"
#include "stateutilities.h"
#include "gamestate.h"
#include "winds.h"
#include "testai.h"

using namespace Mahjong;

TEST(Setup, SeatWind){
  EXPECT_EQ(GetSeat(0,0), Wind::East);
  EXPECT_EQ(GetSeat(0,1), Wind::South);
  EXPECT_EQ(GetSeat(0,2), Wind::West);
  EXPECT_EQ(GetSeat(0,3), Wind::North);
}

TEST(Setup, PrevalentWind){
  EXPECT_EQ(GetPrevalentWind(0), Wind::East);
  EXPECT_EQ(GetPrevalentWind(4), Wind::South);
  EXPECT_EQ(GetSeat(4,0), Wind::East);
}

TEST(Setup, DoraIndicator){
  GameState state;
  TesterBot bot;
  for(int i = 0; i < 4; i++){
    state.players[i].controller = &bot;
  }
  ASSERT_NO_THROW(RoundStart(state));
  EXPECT_EQ(state.walls.GetDoras().size(),1);
}

TEST(Setup, Dealing){
  GameState state;
  TesterBot bot;
  for(int i = 0; i < 4; i++){
    state.players[i].controller = &bot;
  }
  ASSERT_NO_THROW(RoundStart(state));
  for(int i = 0; i < 4; i++){
    EXPECT_EQ(state.hands[i].live.size(),13);
  }
  Draw(state);
  EXPECT_EQ(state.currentPlayer,0);
}
