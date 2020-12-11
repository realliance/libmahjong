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

TEST(isBlessingOfMan, 5Han) {
  auto gameState = GameState();
  gameState.hands[3] = Hand(HandFromNotation("123m123p444m111z55m"));
  gameState.hands[3].open = false;

  gameState.turnNum = 1;

  // No calls have occured
  gameState.lastCall = -1;

  gameState.hasRonned[3] = true;

  auto root = breakdownHand(gameState.hands.at(3).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfMan(gameState, 3, branch) == 5) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBlessingOfMan, MustBeARon) {
  auto gameState = GameState();
  gameState.hands[3] = Hand(HandFromNotation("123m123p444m111z55m"));
  gameState.hands[3].open = false;

  gameState.turnNum = 3;

  // No calls have occured
  gameState.lastCall = -1;

  gameState.hasRonned[3] = false;

  auto root = breakdownHand(gameState.hands.at(3).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfMan(gameState, 3, branch) == 5) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfMan, MustBeBeforePlayerFirstTurn) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m123p444m111z55m"));
  gameState.hands[0].open = false;

  gameState.turnNum = 2;

  // No calls have occured
  gameState.lastCall = -1;

  gameState.hasRonned[0] = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfMan(gameState, 0, branch) == 5) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfMan, NoCalledMustHaveOccured) {
  auto gameState = GameState();
  gameState.hands[3] = Hand(HandFromNotation("123m123p444m111z55m"));
  gameState.hands[3].open = false;

  gameState.turnNum = 1;

  gameState.lastCall = 0;

  gameState.hasRonned[3] = true;

  auto root = breakdownHand(gameState.hands.at(3).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfMan(gameState, 3, branch) == 5) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
