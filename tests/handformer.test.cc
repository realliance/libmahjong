#include "handformer.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "pieces.h"
#include "piecetype.h"

namespace mahjong {

TEST(HandFormer, HandFromNotationFormSet) {
  std::vector<Piece> hand = {Piece(kOneBamboo), Piece(kTwoBamboo),
                             Piece(kThreeBamboo)};
  auto result = HandFromNotation("123s");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationFormWithManySuits) {
  std::vector<Piece> hand = {Piece(kOneBamboo), Piece(kTwoBamboo),
                             Piece(kThreeBamboo), Piece(kFourCharacter)};
  auto result = HandFromNotation("123s4m");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationFormRedFives) {
  std::vector<Piece> hand = {Piece(kRedFiveCharacter), Piece(kRedFiveBamboo),
                             Piece(kRedFivePin)};
  auto result = HandFromNotation("0m0s0p");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationFormHonors) {
  std::vector<Piece> hand = {Piece(kEastWind),    Piece(kSouthWind),
                             Piece(kWestWind),    Piece(kNorthWind),
                             Piece(kWhiteDragon), Piece(kGreenDragon),
                             Piece(kRedDragon)};
  auto result = HandFromNotation("1234567z");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationFormTerminals) {
  std::vector<Piece> hand = {Piece(kOneCharacter), Piece(kNineCharacter),
                             Piece(kOneBamboo),    Piece(kNineBamboo),
                             Piece(kOnePin),       Piece(kNinePin)};
  auto result = HandFromNotation("19m19s19p");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationInvalidPiece) {
  std::vector<Piece> hand = {Piece(Piece::kError)};
  auto result = HandFromNotation("7g");
  EXPECT_EQ(result, hand);
}

TEST(HandFormer, HandFromNotationInvalidPieceSet) {
  std::vector<Piece> hand = {Piece(Piece::kError), Piece(Piece::kError),
                             Piece(Piece::kError)};
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
  std::vector<Piece> hand = {Piece(kOneBamboo), Piece(kTwoBamboo),
                             Piece(kThreeBamboo)};
  EXPECT_EQ(HandToNotation(hand), "123s");
}

TEST(HandFormer, HandToNotationMultipleSets) {
  std::vector<Piece> hand = {
      Piece(kOneBamboo),     Piece(kTwoBamboo), Piece(kThreeBamboo),
      Piece(kFourPin),       Piece(kFivePin),   Piece(kSixPin),
      Piece(kNineCharacter), Piece(kEastWind),  Piece(kWestWind),
  };
  EXPECT_EQ(HandToNotation(hand), "13z123s456p9m");
}

TEST(HandFormer, HandToNotationSkipsErrorPieces) {
  std::vector<Piece> hand = {
      Piece(kOneBamboo),
      Piece(Piece::kError),
      Piece(kThreeBamboo),
  };
  EXPECT_EQ(HandToNotation(hand), "13s");
}

TEST(HandFormer, HandToNotationRedFives) {
  std::vector<Piece> hand = {
      Piece(kFiveBamboo),
      Piece(kRedFiveBamboo),
  };
  EXPECT_EQ(HandToNotation(hand), "50s");
}
}  // namespace mahjong
