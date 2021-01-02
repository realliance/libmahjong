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

TEST(isThreeKans, 2Han) {
  auto meldA = Meld { Meld::ConcealedKan, Piece(Piece::ONE_BAMBOO) };
  auto meldB = Meld { Meld::Kan, Piece(Piece::TWO_BAMBOO) };
  auto meldC = Meld { Meld::Kan, Piece(Piece::FIVE_PIN) };

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("666z22m"));
  gameState.hands[0].melds = { meldA, meldB, meldC };
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThreeKans(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeKans, WithOtherMelds) {
  auto meldA = Meld { Meld::ConcealedKan, Piece(Piece::ONE_BAMBOO) };
  auto meldB = Meld { Meld::Kan, Piece(Piece::TWO_BAMBOO) };
  auto meldC = Meld { Meld::Kan, Piece(Piece::FIVE_PIN) };
  auto meldD = Meld { Meld::Chi, Piece(Piece::TWO_BAMBOO) };

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("22m"));
  gameState.hands[0].melds = { meldA, meldB, meldC, meldD };
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThreeKans(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isThreeKans, BadHand) {
  auto meldA = Meld { Meld::ConcealedKan, Piece(Piece::ONE_BAMBOO) };
  auto meldB = Meld { Meld::Kan, Piece(Piece::TWO_BAMBOO) };

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("777p666z22m"));
  gameState.hands[0].melds = { meldA, meldB };
  gameState.hands[0].open = true;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThreeKans(gameState, 0, branch) == 2) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}

TEST(isThreeKans, AllConcealed) {
  auto meldA = Meld { Meld::ConcealedKan, Piece(Piece::ONE_BAMBOO) };
  auto meldB = Meld { Meld::ConcealedKan, Piece(Piece::TWO_BAMBOO) };
  auto meldC = Meld { Meld::ConcealedKan, Piece(Piece::FIVE_PIN) };

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("666z22m"));
  gameState.hands[0].melds = { meldA, meldB, meldC };
  gameState.hands[0].open = false;

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isThreeKans(gameState, 0, branch) == 2) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}
