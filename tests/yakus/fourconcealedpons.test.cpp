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

TEST(isFourConcealedPon, Yakuman) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222p888s666z44m"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFourConcealedPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFourConcealedPon, ConcealedKans) {
  auto meld = Meld{Meld::ConcealedKan,Piece(Piece::FIVE_BAMBOO)};
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222p666z44m"));
  gameState.hands[0].open = false;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFourConcealedPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFourConcealedPon, SingleWait) {
  auto meld = Meld{Meld::ConcealedKan,Piece(Piece::FIVE_BAMBOO)};
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222p666z44m"));
  gameState.hands[0].open = false;
  gameState.hands[0].melds = {meld};

  gameState.pendingPiece = Piece(Piece::FOUR_CHARACTER);

  gameState.hasRonned[0] = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFourConcealedPon(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFourConcealedPon, MustBeConcealed) {
  auto meld = Meld{Meld::Pon,Piece(Piece::FIVE_BAMBOO)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222p666z44m"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meld};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFourConcealedPon(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isFourConcealedPon, MustBeSingleWaitForRon) {
  auto meld = Meld{Meld::ConcealedKan,Piece(Piece::FIVE_BAMBOO)};
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222p666z44m"));
  gameState.hands[0].open = false;
  gameState.hands[0].melds = {meld};

  gameState.pendingPiece = Piece(Piece::TWO_PIN);

  gameState.hasRonned[0] = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFourConcealedPon(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isFourConcealedPon, BadHand) {
  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m222p567s666z44m"));

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFourConcealedPon(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}
