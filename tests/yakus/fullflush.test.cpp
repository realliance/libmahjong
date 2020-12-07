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

TEST(isFullFlush, 5Han) {
  auto meld = Meld {Meld::Pon, Piece(Piece::SIX_CHARACTER)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222m345m99m"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFullFlush(gameState, 0, branch) == 5) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFullFlush, 6Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222m345m666m99m"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFullFlush(gameState, 0, branch) == 6) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFullFlush, BadHandHonors) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222m111z666m99m"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFullFlush(gameState, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isFullFlush, BadHandSuit) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222m111p666m99m"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFullFlush(gameState, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isFullFlush, BadHandFullFlushHonors) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111z222z333z444z55z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFullFlush(gameState, 0, branch) > 0) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}