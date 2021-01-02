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

TEST(isAllPons, 2Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222p888s666z44m"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isAllPons(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, WithKans) {
  auto meld = Meld { Meld::Kan, Piece(Piece::FIVE_PIN )};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222p666z44m"));
  gameState.hands[0].melds = { meld };
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isAllPons(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, ConcealedKan) {
  auto meld = Meld { Meld::ConcealedKan, Piece(Piece::FIVE_PIN )};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222p666z44m"));
  gameState.hands[0].melds = { meld };
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isAllPons(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isAllPons, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222p666z789m44m"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isAllPons(gameState, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
