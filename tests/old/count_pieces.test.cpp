#include <gtest/gtest.h>
#include "gamestate.h"
#include "manager.h"
#include "stateutilities.h"

using namespace Mahjong;

TEST(CountPieces, CountPieces) {
  GameState state;
  state.hands[0].live = {Piece::ONE_CHARACTER,Piece::FOUR_PIN,Piece::WHITE_DRAGON,Piece::WHITE_DRAGON};

  EXPECT_EQ(CountPieces(state,0,Piece::ONE_CHARACTER),1);
  EXPECT_EQ(CountPieces(state,0,Piece::FOUR_PIN),1);
  EXPECT_EQ(CountPieces(state,0,Piece::WHITE_DRAGON),2);
  EXPECT_EQ(CountPieces(state,0,Piece::GREEN_DRAGON),0);
}
