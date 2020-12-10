#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "handformer.h"
#include "piecetype.h"

using Mahjong::Piece;
using Mahjong::HandFromNotation;
using Mahjong::IsValidNotation;
using Mahjong::HandToNotation;

TEST(HandFormer, HandFromNotationFormSet) {
  std::vector<Piece> hand = {Piece(Piece::ONE_BAMBOO), Piece(Piece::TWO_BAMBOO), Piece(Piece::THREE_BAMBOO)};
  auto result = HandFromNotation("123s");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationFormWithManySuits) {
  std::vector<Piece> hand = {
    Piece(Piece::ONE_BAMBOO),
    Piece(Piece::TWO_BAMBOO),
    Piece(Piece::THREE_BAMBOO),
    Piece(Piece::FOUR_CHARACTER)};
  auto result = HandFromNotation("123s4m");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationFormRedFives) {
  std::vector<Piece> hand = {
    Piece(Piece::RED_FIVE_CHARACTER),
    Piece(Piece::RED_FIVE_BAMBOO),
    Piece(Piece::RED_FIVE_PIN)};
  auto result = HandFromNotation("0m0s0p");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationFormHonors) {
  std::vector<Piece> hand = {
    Piece(Piece::EAST_WIND),
    Piece(Piece::SOUTH_WIND),
    Piece(Piece::WEST_WIND),
    Piece(Piece::NORTH_WIND),
    Piece(Piece::WHITE_DRAGON),
    Piece(Piece::GREEN_DRAGON),
    Piece(Piece::RED_DRAGON)};
  auto result = HandFromNotation("1234567z");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationFormTerminals) {
  std::vector<Piece> hand = {
    Piece(Piece::ONE_CHARACTER),
    Piece(Piece::NINE_CHARACTER),
    Piece(Piece::ONE_BAMBOO),
    Piece(Piece::NINE_BAMBOO),
    Piece(Piece::ONE_PIN),
    Piece(Piece::NINE_PIN)};
  auto result = HandFromNotation("19m19s19p");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationInvalidPiece) {
  std::vector<Piece> hand = {
    Piece(Piece::ERROR)};
  auto result = HandFromNotation("7g");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationInvalidPieceSet) {
  std::vector<Piece> hand = {
    Piece(Piece::ERROR),
    Piece(Piece::ERROR),
    Piece(Piece::ERROR)};
  auto result = HandFromNotation("799g");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, IsValidNotationWithValidNotations) {
  EXPECT_TRUE(IsValidNotation("123s"));
  EXPECT_TRUE(IsValidNotation("123s456m"));
  EXPECT_TRUE(IsValidNotation("11z22z33z"));
  EXPECT_TRUE(IsValidNotation("1p2p3p4p"));
  EXPECT_TRUE(IsValidNotation("50m"));

  // "Nothing" defaults to true
  EXPECT_TRUE(IsValidNotation(""));
}

TEST(HandFormer, IsValidNotationWithInvaildNotations) {
  EXPECT_FALSE(IsValidNotation("123"));
  EXPECT_FALSE(IsValidNotation("123g456m"));
  EXPECT_FALSE(IsValidNotation("11z22z33"));
  EXPECT_FALSE(IsValidNotation("@#-s2712s"));
  EXPECT_FALSE(IsValidNotation("8z"));
}

TEST(HandFormer, HandToNotationSingleSet) {
  std::vector<Piece> hand = {Piece(Piece::ONE_BAMBOO), Piece(Piece::TWO_BAMBOO), Piece(Piece::THREE_BAMBOO)};
  EXPECT_EQ(HandToNotation(hand), "123s");
}

TEST(HandFormer, HandToNotationMultipleSets) {
  std::vector<Piece> hand = {
    Piece(Piece::ONE_BAMBOO), Piece(Piece::TWO_BAMBOO), Piece(Piece::THREE_BAMBOO),
    Piece(Piece::FOUR_PIN), Piece(Piece::FIVE_PIN), Piece(Piece::SIX_PIN),
    Piece(Piece::NINE_CHARACTER), Piece(Piece::EAST_WIND), Piece(Piece::WEST_WIND),
  };
  EXPECT_EQ(HandToNotation(hand), "13z123s456p9m");
}

TEST(HandFormer, HandToNotationSkipsErrorPieces) {
  std::vector<Piece> hand = {
    Piece(Piece::ONE_BAMBOO), Piece(Piece::ERROR), Piece(Piece::THREE_BAMBOO),
  };
  EXPECT_EQ(HandToNotation(hand), "13s");
}

TEST(HandFormer, HandToNotationRedFives) {
  std::vector<Piece> hand = {
    Piece(Piece::FIVE_BAMBOO), Piece(Piece::RED_FIVE_BAMBOO),
  };
  EXPECT_EQ(HandToNotation(hand), "50s");
}