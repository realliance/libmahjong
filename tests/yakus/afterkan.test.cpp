#include <gtest/gtest.h>
#include "hands.h"
#include "hand.h"
#include "gamestate.h"
#include "handformer.h"
#include "handnode.h"
#include "analysis.h"
#include "statefunctions.h"

using Mahjong::GameState;
using Mahjong::Hand;
using Mahjong::HandFromNotation;
using Mahjong::breakdownHand;
using Mahjong::Node;
using Mahjong::Piece;

TEST(isAfterAKan, 1Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m789m1111z999s55z"));
  gameState.currentPlayer = 0;
  
  gameState.prevState = Mahjong::Replacement;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isAfterAKan(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAfterAKan, DoesntApply) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m789m1111z999s55z"));
  gameState.currentPlayer = 0;

  gameState.prevState = Mahjong::Pon;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isAfterAKan(gameState, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isAfterAKan, WrongPlayer) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m789m1111z999s55z"));
  gameState.currentPlayer = 2;

  gameState.prevState = Mahjong::Replacement;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isAfterAKan(gameState, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}