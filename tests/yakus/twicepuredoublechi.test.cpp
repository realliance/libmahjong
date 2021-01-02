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
using Mahjong::Meld;

TEST(isTwicePureDoubleChi, 3Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("789p789p234m234m11z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isTwicePureDoubleChi(gameState, 0, branch) == 3) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTwicePureDoubleChi, MustBeConcealed) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("789p789p234m234m11z"));
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isTwicePureDoubleChi(gameState, 0, branch) == 3) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isTwicePureDoubleChi, PureDoubleChowIncompatible) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("789p789p234m234m11z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isTwicePureDoubleChi(gameState, 0, branch) == 3 && Mahjong::isPureDoubleChi(gameState, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isTwicePureDoubleChi, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("789p789p234m567m11z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isTwicePureDoubleChi(gameState, 0, branch) == 3) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
