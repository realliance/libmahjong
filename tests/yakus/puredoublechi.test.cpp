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

TEST(isPureDoubleChi, 1Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("234m234m555p888s88p"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isPureDoubleChi(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isPureDoubleChi, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("234m123m555p888s88p"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isPureDoubleChi(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isPureDoubleChi, MustBeConcealed) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("234m234m555p888s88p"));
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isPureDoubleChi(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
