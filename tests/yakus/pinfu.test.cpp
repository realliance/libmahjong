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

TEST(isPinfu, 1Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456p234s678z44m"));
  gameState.hands[0].open = false;
  gameState.pendingPiece = Piece(Piece::FOUR_PIN);

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isPinfu(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isPinfu, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m555p234s678z44m"));
  gameState.hands[0].open = false;
  gameState.pendingPiece = Piece(Piece::TWO_BAMBOO);

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isPinfu(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isPinfu, CantBeOpen) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456p234s678z44m"));
  gameState.hands[0].open = true;
  gameState.pendingPiece = Piece(Piece::FOUR_PIN);

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isPinfu(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isPinfu, NeedTwoWait) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456p234s678z44m"));
  gameState.hands[0].open = false;
  gameState.pendingPiece = Piece(Piece::FIVE_PIN);

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isPinfu(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
