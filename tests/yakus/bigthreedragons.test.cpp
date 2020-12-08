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

TEST(isBigThreeDragons, Yakuman) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555z666z777z222m55p"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBigThreeDragons(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBigThreeDragons, CanBeOpen) {
  auto meldA = Meld{Meld::Pon,Piece(Piece::RED_DRAGON)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555z777z222m55p"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meldA};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBigThreeDragons(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBigThreeDragons, BadHand) {
  auto meldA = Meld{Meld::Pon,Piece(Piece::RED_DRAGON)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("555z777m222m55p"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meldA};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBigThreeDragons(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}