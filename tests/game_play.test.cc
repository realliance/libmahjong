#include <gtest/gtest.h>
#include "statefunctions.h"
#include "stateutilities.h"
#include "decisionfunction.h"
#include "gamestate.h"
#include "winds.h"
#include "testai.h"
#include "hands.h"
#include "pieces.h"

using namespace mahjong;

inline bool operator==(const mahjong::Event& lhs, const mahjong::Event& rhs) {
  return lhs.type == rhs.type &&
    lhs.player == rhs.player &&
    lhs.piece == rhs.piece &&
    lhs.decision == rhs.decision;
}

TEST(GamePlay, Discard){
  GameState state;
  state.overrideWall = {kSixBamboo};
  TesterBot bot;
  for(int i = 0; i < 4; i++){
    state.players[i].controller = &bot;
  }
  RoundStart(state);
  Draw(state);
  Event e = {
      Event::kDiscard,
      0,
      kSixBamboo,
      true
    };
  bot.AddEvents({e});
  ASSERT_NO_THROW(PlayerHand(state));
  EXPECT_TRUE(e == bot.GetEvents()[0]);
  EXPECT_TRUE(state.hands[0].discards[0] == kSixBamboo);
}





TEST(GamePlay, Furiten){
  GameState state;
  state.pendingPiece = kRedDragon;
  state.hands[0].live = {kRedDragon, kRedDragon,kRedDragon};
  ASSERT_TRUE(isComplete(state,0));
  state.hands[0].live = {kRedDragon,kRedDragon};
  EXPECT_FALSE(CanRon(state,0));
}
