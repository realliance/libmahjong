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

TEST(isHalfFlush, 2Han) {
  auto meld = Meld {Meld::Pon, Piece(Piece::SIX_CHARACTER)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("11m234m111z222z"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isHalfFlush(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isHalfFlush, 3Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("11m234m777m111z222z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isHalfFlush(gameState, 0, branch) == 3) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isHalfFlush, BadHand) {
  auto meld = Meld {Meld::Pon, Piece(Piece::SIX_CHARACTER)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("11m234m111p222z"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isHalfFlush(gameState, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isHalfFlush, FullFlushIncompatible) {
  auto meld = Meld {Meld::Pon, Piece(Piece::SIX_CHARACTER)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("11m234m888m777m"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isHalfFlush(gameState, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}