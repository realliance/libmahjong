#include <gtest/gtest.h>
#include "hands.h"
#include "hand.h"
#include "gamestate.h"
#include "handformer.h"

using Mahjong::Hand;
using Mahjong::GameState;
using Mahjong::HandFromNotation;

TEST(isRiichi, 1Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  gameState.hands[0].riichi = true;

  gameState.hands[0].riichiRound = 7;
  gameState.turnNum = 15;
  gameState.lastCall = 2;

  EXPECT_EQ(isRiichi(gameState, 0), 1);
}

TEST(isRiichi, Ippatsu) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  gameState.hands[0].riichi = true;

  gameState.hands[0].riichiRound = 14;
  gameState.turnNum = 15;
  gameState.lastCall = 2;

  EXPECT_EQ(isRiichi(gameState, 0), 2);
}

TEST(isRiichi, DoubleRiichi) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  gameState.hands[0].riichi = true;

  gameState.hands[0].riichiRound = 1;
  gameState.turnNum = 2;
  gameState.lastCall = -1;

  EXPECT_EQ(isRiichi(gameState, 0), 3);
}

TEST(isRiichi, NoRiichi) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555m555p555s111z44m"));
  gameState.hands[0].riichi = false;

  gameState.turnNum = 2;
  gameState.lastCall = -1;

  EXPECT_EQ(isRiichi(gameState, 0), 0);
}
