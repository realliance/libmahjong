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

TEST(isLittleThreeDragons, 2Han) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555z666z111m222p77z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isLittleThreeDragons(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isLittleThreeDragons, WhenOpen) {
  auto meld = Meld {Meld::Pon, Piece(Piece::WHITE_DRAGON)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("666z111m222p77z"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isLittleThreeDragons(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isLittleThreeDragons, BadHand) {
  auto meld = Meld {Meld::Pon, Piece(Piece::WHITE_DRAGON)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("666z111m222p77s"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isLittleThreeDragons(gameState, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
