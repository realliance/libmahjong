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

TEST(isThreeConcealedPons, 2Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m111p111s666z44m"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThreeConcealedPons(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, PonsMustBeConcealed) {
  auto meld = Meld();
  meld.type = Meld::Chi;
  meld.start = Piece(Piece::TWO_BAMBOO);

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m111p111s44m"));
  gameState.hands[0].melds.push_back(meld);
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThreeConcealedPons(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeConcealedPons, PonsMustBeConcealedNegative) {
  auto meld = Meld();
  meld.type = Meld::Pon;
  meld.start = Piece(Piece::ONE_BAMBOO);

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m111p234s44m"));
  gameState.hands[0].melds.push_back(meld);
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThreeConcealedPons(gameState, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isThreeConcealedPons, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m111p234567s44m"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThreeConcealedPons(gameState, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
