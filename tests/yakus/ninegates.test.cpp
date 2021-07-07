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

TEST(isNineGates, Yakuman) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("1112345678999m7m"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isNineGates(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isNineGates, MustBeConcealed) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("1112345678999m7m"));
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isNineGates(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isNineGates, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("1112345678999m1p"));
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isNineGates(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}