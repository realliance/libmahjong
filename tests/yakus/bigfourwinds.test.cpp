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

TEST(isBigFourWinds, Yakuman) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111z222z333z444z77m"));
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBigFourWinds(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBigFourWinds, CanBeOpen) {
  auto meldA = Meld{Meld::Pon,Piece(Piece::EAST_WIND)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("222z333z444z77m"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meldA};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBigFourWinds(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isBigFourWinds, BadHand) {
  auto meldA = Meld{Meld::Pon,Piece(Piece::EAST_WIND)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("222z333z444p77m"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meldA};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isBigFourWinds(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}