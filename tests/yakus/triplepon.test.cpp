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

TEST(isTriplePon, 2Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m111p111s666z44m"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isTriplePon(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTriplePon, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m111p222m666z44m"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isTriplePon(gameState, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}