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

TEST(scoreHand, YakuWithRon) {
  const uint8_t EXPECTED_HAN = 4;
  const uint8_t EXPECTED_FU = 30;
  const uint8_t EXPECTED_YAKUMAN = 0;

  auto meldA = Meld{Meld::Pon, Piece(Piece::WHITE_DRAGON)};
  auto meldB = Meld{Meld::Pon, Piece(Piece::RED_DRAGON)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("234p345s66z"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meldA,meldB};
  gameState.hasRonned[0] = true;

  gameState.pendingPiece = Piece(Piece::FIVE_BAMBOO);

  auto score = Mahjong::scoreHand(gameState, 0);

  EXPECT_EQ(score.han, EXPECTED_HAN);
  EXPECT_EQ(score.fu, EXPECTED_FU);
  EXPECT_EQ(score.yakuman, EXPECTED_YAKUMAN);
}

TEST(scoreHand, YakumanSelfDraw) {
  const uint8_t EXPECTED_HAN = 6;
  const uint8_t EXPECTED_FU = 34;
  const uint8_t EXPECTED_YAKUMAN = 1;

  auto meldA = Meld{Meld::Pon, Piece(Piece::ONE_BAMBOO)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("111m99m111p999p"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meldA};

  auto score = Mahjong::scoreHand(gameState, 0);

  EXPECT_EQ(score.han, EXPECTED_HAN);
  EXPECT_EQ(score.fu, EXPECTED_FU);
  EXPECT_EQ(score.yakuman, EXPECTED_YAKUMAN);
}

TEST(scoreHand, YakuWithDora) {
  const uint8_t EXPECTED_HAN = 4 + 3;
  const uint8_t EXPECTED_FU = 30;
  const uint8_t EXPECTED_YAKUMAN = 0;

  auto meldA = Meld{Meld::Pon, Piece(Piece::WHITE_DRAGON)};
  auto meldB = Meld{Meld::Pon, Piece(Piece::RED_DRAGON)};

  auto gameState = GameState();
  gameState.hands[0] = Hand(HandFromNotation("234p345s66z"));
  gameState.hands[0].open = true;
  gameState.hands[0].melds = {meldA,meldB};
  gameState.hasRonned[0] = true;
  gameState.walls.deadWall = {Piece(Piece::GREEN_DRAGON)};

  gameState.pendingPiece = Piece(Piece::FIVE_BAMBOO);

  auto score = Mahjong::scoreHand(gameState, 0);

  EXPECT_EQ(score.han, EXPECTED_HAN);
  EXPECT_EQ(score.fu, EXPECTED_FU);
  EXPECT_EQ(score.yakuman, EXPECTED_YAKUMAN);
}