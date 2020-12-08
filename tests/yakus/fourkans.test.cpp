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

TEST(isFourKans, Yakuman) {
  auto meldA = Meld{Meld::ConcealedKan,Piece(Piece::THREE_BAMBOO)};
  auto meldB = Meld{Meld::ConcealedKan,Piece(Piece::EIGHT_PIN)};
  auto meldC = Meld{Meld::ConcealedKan,Piece(Piece::NINE_CHARACTER)};
  auto meldD = Meld{Meld::ConcealedKan,Piece(Piece::TWO_BAMBOO)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("44m"));
  gameState.hands[0].melds = {meldA,meldB,meldC,meldD};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFourKans(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFourKans, KansAndConcealedKans) {
  auto meldA = Meld{Meld::ConcealedKan,Piece(Piece::THREE_BAMBOO)};
  auto meldB = Meld{Meld::ConcealedKan,Piece(Piece::EIGHT_PIN)};
  auto meldC = Meld{Meld::Kan,Piece(Piece::NINE_CHARACTER)};
  auto meldD = Meld{Meld::ConcealedKan,Piece(Piece::TWO_BAMBOO)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("44m"));
  gameState.hands[0].melds = {meldA,meldB,meldC,meldD};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFourKans(gameState, 0, branch) == 1) {
      SUCCEED();
      return;
    }
  }
  FAIL();
}

TEST(isFourKans, BadHand) {
  auto meldA = Meld{Meld::ConcealedKan,Piece(Piece::THREE_BAMBOO)};
  auto meldB = Meld{Meld::Pon,Piece(Piece::EIGHT_PIN)};
  auto meldC = Meld{Meld::Kan,Piece(Piece::NINE_CHARACTER)};
  auto meldD = Meld{Meld::ConcealedKan,Piece(Piece::TWO_BAMBOO)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("44m"));
  gameState.hands[0].melds = {meldA,meldB,meldC,meldD};

  auto root = breakdownHand(gameState.hands.at(0).live);

  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (Mahjong::isFourKans(gameState, 0, branch) == 1) {
      FAIL();
      return;
    }
  }
  SUCCEED();
}