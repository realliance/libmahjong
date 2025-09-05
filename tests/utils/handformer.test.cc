#include "utils/handformer.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong {

TEST(PlayerFormer, HandFromNotationFormSet) {
  const std::vector<Piece> hand = {Piece(kOneBamboo), Piece(kTwoBamboo),
                                   Piece(kThreeBamboo)};
  auto result = HandFromNotation("123s");
  EXPECT_EQ(result, hand);
}

TEST(PlayerFormer, HandFromNotationFormWithManySuits) {
  const std::vector<Piece> hand = {Piece(kOneBamboo), Piece(kTwoBamboo),
                                   Piece(kThreeBamboo), Piece(kFourCharacter)};
  auto result = HandFromNotation("123s4m");
  EXPECT_EQ(result, hand);
}

TEST(PlayerFormer, HandFromNotationFormRedFives) {
  const std::vector<Piece> hand = {Piece(kRedFiveCharacter),
                                   Piece(kRedFiveBamboo), Piece(kRedFivePin)};
  auto result = HandFromNotation("0m0s0p");
  EXPECT_EQ(result, hand);
}

TEST(PlayerFormer, HandFromNotationFormHonors) {
  const std::vector<Piece> hand = {Piece(kEastWind),    Piece(kSouthWind),
                                   Piece(kWestWind),    Piece(kNorthWind),
                                   Piece(kWhiteDragon), Piece(kGreenDragon),
                                   Piece(kRedDragon)};
  auto result = HandFromNotation("1234567z");
  EXPECT_EQ(result, hand);
}

TEST(PlayerFormer, HandFromNotationFormTerminals) {
  const std::vector<Piece> hand = {Piece(kOneCharacter), Piece(kNineCharacter),
                                   Piece(kOneBamboo),    Piece(kNineBamboo),
                                   Piece(kOnePin),       Piece(kNinePin)};
  auto result = HandFromNotation("19m19s19p");
  EXPECT_EQ(result, hand);
}

TEST(PlayerFormer, HandFromNotationInvalidPiece) {
  const std::vector<Piece> hand = {Piece(Piece::kError)};
  auto result = HandFromNotation("7g");
  EXPECT_EQ(result, hand);
}

TEST(PlayerFormer, HandFromNotationInvalidPieceSet) {
  const std::vector<Piece> hand = {Piece(Piece::kError), Piece(Piece::kError),
                                   Piece(Piece::kError)};
  auto result = HandFromNotation("799g");
  EXPECT_EQ(result, hand);
}

TEST(PlayerFormer, IsValidNotationWithValidNotations) {
  EXPECT_TRUE(IsValidNotation("123s"));
  EXPECT_TRUE(IsValidNotation("123s456m"));
  EXPECT_TRUE(IsValidNotation("11z22z33z"));
  EXPECT_TRUE(IsValidNotation("1p2p3p4p"));
  EXPECT_TRUE(IsValidNotation("50m"));

  // "Nothing" defaults to true
  EXPECT_TRUE(IsValidNotation(""));
}

TEST(PlayerFormer, IsValidNotationWithInvaildNotations) {
  EXPECT_FALSE(IsValidNotation("123"));
  EXPECT_FALSE(IsValidNotation("123g456m"));
  EXPECT_FALSE(IsValidNotation("11z22z33"));
  EXPECT_FALSE(IsValidNotation("@#-s2712s"));
  EXPECT_FALSE(IsValidNotation("8z"));
}

TEST(PlayerFormer, PlayerToNotationSingleSet) {
  const std::vector<Piece> hand = {Piece(kOneBamboo), Piece(kTwoBamboo),
                                   Piece(kThreeBamboo)};
  EXPECT_EQ(PlayerToNotation(hand), "123s");
}

TEST(PlayerFormer, PlayerToNotationMultipleSets) {
  const std::vector<Piece> hand = {
      Piece(kOneBamboo),     Piece(kTwoBamboo), Piece(kThreeBamboo),
      Piece(kFourPin),       Piece(kFivePin),   Piece(kSixPin),
      Piece(kNineCharacter), Piece(kEastWind),  Piece(kWestWind),
  };
  EXPECT_EQ(PlayerToNotation(hand), "13z123s456p9m");
}

TEST(PlayerFormer, PlayerToNotationSkipsErrorPieces) {
  const std::vector<Piece> hand = {
      Piece(kOneBamboo),
      Piece(Piece::kError),
      Piece(kThreeBamboo),
  };
  EXPECT_EQ(PlayerToNotation(hand), "13s");
}

TEST(PlayerFormer, PlayerToNotationRedFives) {
  const std::vector<Piece> hand = {
      Piece(kFiveBamboo),
      Piece(kRedFiveBamboo),
  };
  EXPECT_EQ(PlayerToNotation(hand), "50s");
}
}  // namespace mahjong
