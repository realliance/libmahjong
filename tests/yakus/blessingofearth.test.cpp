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

TEST(isBlessingOfEarth, Yakuman) {
  auto gameState = GameState();
  gameState.hands[2] = Hand(HandFromNotation("111p222m333s444p11z"));
  gameState.hands[2].open = false;

  gameState.turnNum = 2;

  // No calls have occured
  gameState.lastCall = -1;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfEarth(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBlessingOfEarth, CannotBeInterrupted) {
  auto gameState = GameState();
  gameState.hands[2] = Hand(HandFromNotation("111p222m333s444p11z"));
  gameState.hands[2].open = false;

  gameState.turnNum = 2;

  // No calls have occured
  gameState.lastCall = 1;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfEarth(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfEarth, TooLate) {
  auto gameState = GameState();
  gameState.hands[2] = Hand(HandFromNotation("111p222m333s444p11z"));
  gameState.hands[2].open = false;

  gameState.turnNum = 6;

  // No calls have occured
  gameState.lastCall = -1;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfEarth(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfEarth, BadHand) {
  auto gameState = GameState();
  gameState.hands[2] = Hand(HandFromNotation("111p225m333s444p11z"));
  gameState.hands[2].open = false;

  gameState.turnNum = 2;

  // No calls have occured
  gameState.lastCall = -1;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfEarth(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}