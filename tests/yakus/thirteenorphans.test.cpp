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

TEST(isThirteenOrphans, Yakuman) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("19p19s19m1234567z7z"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThirteenOrphans(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThirteenOrphans, MustBeConcealed) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("19p19s19m1234567z7z"));
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThirteenOrphans(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isThirteenOrphans, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("19p29s19m1234567z7z"));
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThirteenOrphans(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}