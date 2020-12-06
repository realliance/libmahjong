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

TEST(isWindOrDragonPon, SeatWind) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m444z55z"));
  gameState.roundNum = 1;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, SeatWindKan) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m4444z55z"));
  gameState.roundNum = 1;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, PrevalentWind) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m111z55z"));
  gameState.roundNum = 1;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, PrevalentWindKan) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m1111z55z"));
  gameState.roundNum = 1;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, Dealer2Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m111z55z"));
  gameState.roundNum = 0;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, Dealer2HanKan) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m1111z55z"));
  gameState.roundNum = 0;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isWindOrDragonPon, NotSeatOrPrevalent) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m222z55z"));
  gameState.roundNum = 0;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isWindOrDragonPon, NoWind) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("123m456m666m55z"));
  gameState.roundNum = 0;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isWindOrDragonPon(gameState, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}