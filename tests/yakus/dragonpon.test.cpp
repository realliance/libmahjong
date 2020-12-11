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

TEST(isWindOrDragonPon, WhiteDragon) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m555z11z"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, GreenDragon) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m666z11z"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, RedDragon) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m777z11z"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, Kan) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m7777z11z"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, CanWhenOpen) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m777z11z"));
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m222z11z"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
