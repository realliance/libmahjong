#include <gtest/gtest.h>
#include "gamestate.h"
#include "manager.h"
#include "piecetype.h"
#include "stateutilities.h"

using namespace Mahjong;

TEST(RemovePieces, RemoveCorrectAmount) {
  GameState state;
  state.hands[0].live = {Piece::ONE_CHARACTER,Piece::FOUR_PIN,Piece::WHITE_DRAGON,Piece::WHITE_DRAGON};

  EXPECT_EQ(RemovePieces(state,0,Piece::ONE_CHARACTER,1),1);
  EXPECT_EQ(state.hands[0].live.size(),3);
}

TEST(RemovePieces, RemoveOnlyTheOnesAvailable) {
  GameState state;
  state.hands[0].live = {Piece::ONE_CHARACTER,Piece::FOUR_PIN,Piece::WHITE_DRAGON,Piece::WHITE_DRAGON};

  EXPECT_EQ(RemovePieces(state,0,Piece::FOUR_PIN,2),1);
  EXPECT_EQ(state.hands[0].live.size(),3);
}

TEST(RemovePieces, OnlyRemovesOne) {
  GameState state;
  state.hands[0].live = {Piece::ONE_CHARACTER,Piece::FOUR_PIN,Piece::WHITE_DRAGON,Piece::WHITE_DRAGON};

  EXPECT_EQ(RemovePieces(state,0,Piece::WHITE_DRAGON,1),1);
  EXPECT_EQ(state.hands[0].live.back(),Piece::WHITE_DRAGON);
}

TEST(RemovePieces, RemovesZeroWhenPieceDoesntExist) {
  GameState state;
  state.hands[0].live = {Piece::ONE_CHARACTER,Piece::FOUR_PIN,Piece::WHITE_DRAGON,Piece::WHITE_DRAGON};

  EXPECT_EQ(RemovePieces(state,0,Piece::GREEN_DRAGON,8),0);
  EXPECT_EQ(state.hands[0].live.size(),4);
}
