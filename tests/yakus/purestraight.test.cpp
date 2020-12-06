#include <gtest/gtest.h>
#include "hands.h"
#include "hand.h"
#include "gamestate.h"
#include "handformer.h"
#include "handnode.h"
#include "analysis.h"

using Mahjong::GameState;
using Mahjong::Hand;
using Mahjong::HandFromNotation;
using Mahjong::breakdownHand;
using Mahjong::Node;
using Mahjong::Piece;

TEST(isPureStraight, Open) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m789m555p11z"));
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isPureStraight(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isPureStraight, Closed) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m789m555p11z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isPureStraight(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isPureStraight, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m999m555p11z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isPureStraight(gameState, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}