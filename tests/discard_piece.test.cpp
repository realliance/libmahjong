#include <gtest/gtest.h>
#include "gamestate.h"
#include "controllermanager.h"
#include "stateutilities.h"

using namespace Mahjong;

TEST(DiscardPiece, DiscardsCorrectPiece) {
  GameState state;
  state.hands[0].live = {Piece::ONE_CHARACTER,Piece::FOUR_PIN,Piece::WHITE_DRAGON,Piece::WHITE_DRAGON};

  DiscardPiece(state,0,Piece::ONE_CHARACTER);
  EXPECT_EQ(state.hands[0].discards[0],Piece::ONE_CHARACTER);
}

TEST(DiscardPiece, DoesntDiscardInCorrectPiece) {
  GameState state;
  state.hands[0].live = {Piece::ONE_CHARACTER,Piece::FOUR_PIN,Piece::WHITE_DRAGON,Piece::WHITE_DRAGON};

  DiscardPiece(state,0,Piece::GREEN_DRAGON);
  EXPECT_EQ(state.hands[0].live.size(),4);
}
