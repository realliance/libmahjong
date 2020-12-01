#include <gtest/gtest.h>
#include "gamestate.h"
#include "manager.h"
#include "decisionfunction.h"

using namespace Mahjong;

TEST(CanChi, EastShouldChiOnNorth) {
  GameState state;
  state.pendingPiece = Piece::ONE_CHARACTER;
  state.currentPlayer = 3;
  state.hands[0].live= {Piece::ONE_CHARACTER,Piece::TWO_CHARACTER,Piece::THREE_CHARACTER};
  EXPECT_TRUE(CanChi(state,0));
  state.pendingPiece = Piece::TWO_CHARACTER;
  EXPECT_TRUE(CanChi(state,0));
  state.pendingPiece = Piece::THREE_CHARACTER;
  EXPECT_TRUE(CanChi(state,0));
  state.pendingPiece = Piece::FOUR_CHARACTER;
  EXPECT_TRUE(CanChi(state,0));
}

TEST(CanChi, EastShouldNotChiOnSouth) {
  GameState state;
  state.pendingPiece = Piece::ONE_CHARACTER;
  state.currentPlayer = 1;
  state.hands[0].live= {Piece::ONE_CHARACTER,Piece::TWO_CHARACTER,Piece::THREE_CHARACTER};
  EXPECT_FALSE(CanChi(state,0));
}

TEST(CanChi, NoChiOnHonor) {
  GameState state;
  state.pendingPiece = Piece::ONE_CHARACTER;
  state.currentPlayer = 3;
  state.hands[0].live= {Piece::ONE_CHARACTER,Piece::TWO_CHARACTER,Piece::THREE_CHARACTER};
  state.pendingPiece = Piece::WHITE_DRAGON;
  EXPECT_FALSE(CanChi(state,0));
}
