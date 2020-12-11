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

TEST(isMixedTripleChi, Open) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123p123m123s555p11z"));
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isMixedTripleChi(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isMixedTripleChi, Closed) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123p123m123s555p11z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isMixedTripleChi(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isMixedTripleChi, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123p123p123s555p11z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isMixedTripleChi(gameState, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
