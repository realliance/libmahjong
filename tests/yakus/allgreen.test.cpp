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

TEST(isAllGreen, Yakuman) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("222s234s666s666z88s"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isAllGreen(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllGreen, CanBeOpen) {
  auto meld = Meld{Meld::Chi,Piece(Piece::TWO_BAMBOO)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("222s666s666z88s"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isAllGreen(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllGreen, BadHand) {
  auto meld = Meld{Meld::Chi,Piece(Piece::TWO_BAMBOO)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("222s555s666m88s"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isAllGreen(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}