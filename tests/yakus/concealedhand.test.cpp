#include <gtest/gtest.h>
#include "hands.h"
#include "hand.h"
#include "gamestate.h"
#include "handformer.h"

using Mahjong::Hand;
using Mahjong::GameState;
using Mahjong::HandFromNotation;

TEST(isFullyConcealedHand, 1Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  gameState.hands[0].open = false;
  gameState.currentPlayer = 0;

  EXPECT_EQ(isFullyConcealedHand(gameState, 0), 1);
}

TEST(isFullyConcealedHand, MustTsumo) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  gameState.hands[0].open = false;
  gameState.currentPlayer = 1;

  EXPECT_EQ(isFullyConcealedHand(gameState, 0), 0);
}

TEST(isFullyConcealedHand, MustBeClosedHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  gameState.hands[0].open = true;
  gameState.currentPlayer = 0;

  EXPECT_EQ(isFullyConcealedHand(gameState, 0), 0);
}

TEST(isFullyConcealedHand, MustHavePiecesRemainingInWall) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  gameState.hands[0].open = false;
  gameState.currentPlayer = 0;

  // Empty the Wall
  gameState.walls.livingWalls.clear();

  EXPECT_EQ(isFullyConcealedHand(gameState, 0), 0);
}
