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

TEST(isTerminalsInAllSets, 2Han) {
  auto meld = Meld{Meld::Chi, Piece(Piece::SEVEN_PIN)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m789m111p11s"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isTerminalsInAllSets(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTerminalsInAllSets, 3Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m789m111p789p11s"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isTerminalsInAllSets(gameState, 0, branch) == 3) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isTerminalsInAllSets, BadHand) {
  auto meld = Meld{Meld::Chi, Piece(Piece::SEVEN_PIN)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m789m222p11s"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isTerminalsInAllSets(gameState, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
