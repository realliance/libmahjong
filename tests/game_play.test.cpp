#include <gtest/gtest.h>
#include "statefunctions.h"
#include "stateutilities.h"
#include "decisionfunction.h"
#include "gamestate.h"
#include "winds.h"
#include "testai.h"
#include "hands.h"

using namespace Mahjong;

inline bool operator==(const Mahjong::Event& lhs, const Mahjong::Event& rhs) {
  return lhs.type == rhs.type &&
    lhs.player == rhs.player &&
    lhs.piece == rhs.piece &&
    lhs.decision == rhs.decision;
}

TEST(GamePlay, Discard){
  GameState state;
  state.overrideWall = {Piece::SIX_BAMBOO};
  TesterBot bot;
  for(int i = 0; i < 4; i++){
    state.players[i].controller = &bot;
  }
  RoundStart(state);
  Draw(state);
  Event e = {
      Event::Discard,
      0,
      Piece::SIX_BAMBOO,
      true
    };
  bot.AddEvents({e});
  ASSERT_NO_THROW(PlayerHand(state));
  EXPECT_TRUE(e == bot.GetEvents()[0]);
  EXPECT_TRUE(state.hands[0].discards[0] == Piece::SIX_BAMBOO);
}





TEST(GamePlay, Furiten){
  GameState state;
  state.pendingPiece = Piece::RED_DRAGON;
  state.hands[0].live = {Piece::RED_DRAGON, Piece::RED_DRAGON,Piece::RED_DRAGON};
  ASSERT_TRUE(isComplete(state,0));
  state.hands[0].live = {Piece::RED_DRAGON,Piece::RED_DRAGON};
  EXPECT_FALSE(CanRon(state,0));
}
