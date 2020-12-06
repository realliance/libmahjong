#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <iostream>
#include "handformer.h"
#include "piecetype.h"

using namespace Mahjong;

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
  std::cout << result[0].toStr() << std::endl;
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationInvalidPieceSet) {
  std::vector<Piece> hand = {
    Piece(Piece::ERROR),
    Piece(Piece::ERROR),
    Piece(Piece::ERROR)};
  auto result = HandFromNotation("799g");
  std::cout << result[0].toStr() << std::endl;
  EXPECT_EQ(result, hand);
}