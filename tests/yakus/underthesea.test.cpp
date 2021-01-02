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

TEST(isBottomOfTheSea, 1Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m789m111z999s55z"));
  gameState.walls.livingWalls.clear();

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBottomOfTheSea(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBottomOfTheSea, 1HanRonned) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m789m111z999s55z"));
  gameState.walls.livingWalls.clear();
  gameState.hasRonned[0] = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBottomOfTheSea(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBottomOfTheSea, DoesntApply) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m789m111z999s55z"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBottomOfTheSea(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
