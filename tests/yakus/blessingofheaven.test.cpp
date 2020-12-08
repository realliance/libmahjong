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

TEST(isBlessingOfHeaven, Yakuman) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111p222m333s444p11z"));
  gameState.hands[0].open = false;

  gameState.turnNum = 0;

  // No calls have occured
  gameState.lastCall = -1;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfHeaven(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBlessingOfHeaven, MustBeEastWind) {
  auto gameState = GameState();
  gameState.hands[1] = Hand(HandFromNotation("111p222m333s444p11z"));
  gameState.hands[1].open = false;

  gameState.turnNum = 1;

  // No calls have occured
  gameState.lastCall = -1;

  auto root = breakdownHand(gameState.hands.at(1).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfHeaven(gameState, 1, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfHeaven, NoConcealedKans) {
  auto meld = Meld{Meld::ConcealedKan, Piece(Piece::EIGHT_PIN)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111p222m444p11z"));
  gameState.hands[0].open = false;
  gameState.hands[0].melds = {meld};

  gameState.turnNum = 0;

  // No calls have occured
  gameState.lastCall = 0;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfHeaven(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isBlessingOfHeaven, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111p252m333s444p11z"));
  gameState.hands[0].open = false;

  gameState.turnNum = 0;

  // No calls have occured
  gameState.lastCall = -1;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBlessingOfHeaven(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}