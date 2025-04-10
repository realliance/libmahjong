#include <gtest/gtest.h>
#include <string>

#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {

TEST(PieceType, toInteger) {
  EXPECT_EQ(Piece(Piece::kEastWind).toUint8_t(), 1);
  EXPECT_EQ(Piece(Piece::kGreenDragon).toUint8_t(), 7);
  EXPECT_EQ(Piece(Piece::kRedFiveCharacter).toUint8_t(), 101);
  EXPECT_EQ(Piece(Piece::kRedFivePin).toUint8_t(), 69);
  EXPECT_EQ(Piece(Piece::kRedFiveBamboo).toUint8_t(), 37);
  EXPECT_EQ(Piece(Piece::kNineCharacter).toUint8_t(), 233);
  EXPECT_EQ(Piece(Piece::kNinePin).toUint8_t(), 201);
  EXPECT_EQ(Piece(Piece::kNineBamboo).toUint8_t(), 169);
  EXPECT_EQ(Piece(Piece::kOneCharacter).toUint8_t(), 225);
  EXPECT_EQ(Piece(Piece::kOnePin).toUint8_t(), 193);
  EXPECT_EQ(Piece(Piece::kOneBamboo).toUint8_t(), 161);
}

TEST(PieceType, rawValue) {
  EXPECT_EQ(Piece(Piece::kEastWind).raw_value(), 1);
  EXPECT_EQ(Piece(Piece::kGreenDragon).raw_value(), 7);
  EXPECT_EQ(Piece(Piece::kRedFiveCharacter).raw_value(), 117);
  EXPECT_EQ(Piece(Piece::kRedFivePin).raw_value(), 85);
  EXPECT_EQ(Piece(Piece::kRedFiveBamboo).raw_value(), 53);
  EXPECT_EQ(Piece(Piece::kNineCharacter).raw_value(), 233);
  EXPECT_EQ(Piece(Piece::kNinePin).raw_value(), 201);
  EXPECT_EQ(Piece(Piece::kNineBamboo).raw_value(), 169);
  EXPECT_EQ(Piece(Piece::kOneCharacter).raw_value(), 225);
  EXPECT_EQ(Piece(Piece::kOnePin).raw_value(), 193);
  EXPECT_EQ(Piece(Piece::kOneBamboo).raw_value(), 161);
}

TEST(PieceType, isHonor) {
  EXPECT_TRUE(Piece(Piece::kEastWind).isHonor());
  EXPECT_TRUE(Piece(Piece::kGreenDragon).isHonor());
  EXPECT_FALSE(Piece(Piece::kRedFiveCharacter).isHonor());
  EXPECT_FALSE(Piece(Piece::kRedFivePin).isHonor());
  EXPECT_FALSE(Piece(Piece::kRedFiveBamboo).isHonor());
  EXPECT_FALSE(Piece(Piece::kNineCharacter).isHonor());
  EXPECT_FALSE(Piece(Piece::kNinePin).isHonor());
  EXPECT_FALSE(Piece(Piece::kNineBamboo).isHonor());
  EXPECT_FALSE(Piece(Piece::kOneCharacter).isHonor());
  EXPECT_FALSE(Piece(Piece::kOnePin).isHonor());
  EXPECT_FALSE(Piece(Piece::kOneBamboo).isHonor());
}

TEST(PieceType, isTerminal) {
  EXPECT_FALSE(Piece(Piece::kEastWind).isTerminal());
  EXPECT_FALSE(Piece(Piece::kGreenDragon).isTerminal());
  EXPECT_FALSE(Piece(Piece::kRedFiveCharacter).isTerminal());
  EXPECT_FALSE(Piece(Piece::kRedFivePin).isTerminal());
  EXPECT_FALSE(Piece(Piece::kRedFiveBamboo).isTerminal());
  EXPECT_TRUE(Piece(Piece::kNineCharacter).isTerminal());
  EXPECT_TRUE(Piece(Piece::kNinePin).isTerminal());
  EXPECT_TRUE(Piece(Piece::kNineBamboo).isTerminal());
  EXPECT_TRUE(Piece(Piece::kOneCharacter).isTerminal());
  EXPECT_TRUE(Piece(Piece::kOnePin).isTerminal());
  EXPECT_TRUE(Piece(Piece::kOneBamboo).isTerminal());
}

TEST(PieceType, getSuit) {
  EXPECT_EQ(Piece(Piece::kEastWind).getSuit(), Piece::kHonorSuit);
  EXPECT_EQ(Piece(Piece::kGreenDragon).getSuit(), Piece::kHonorSuit);
  EXPECT_EQ(Piece(Piece::kRedFiveCharacter).getSuit(), Piece::kCharacterSuit);
  EXPECT_EQ(Piece(Piece::kRedFivePin).getSuit(), Piece::kPinSuit);
  EXPECT_EQ(Piece(Piece::kRedFiveBamboo).getSuit(), Piece::kBambooSuit);
  EXPECT_EQ(Piece(Piece::kNineCharacter).getSuit(), Piece::kCharacterSuit);
  EXPECT_EQ(Piece(Piece::kNinePin).getSuit(), Piece::kPinSuit);
  EXPECT_EQ(Piece(Piece::kNineBamboo).getSuit(), Piece::kBambooSuit);
  EXPECT_EQ(Piece(Piece::kOneCharacter).getSuit(), Piece::kCharacterSuit);
  EXPECT_EQ(Piece(Piece::kOnePin).getSuit(), Piece::kPinSuit);
  EXPECT_EQ(Piece(Piece::kOneBamboo).getSuit(), Piece::kBambooSuit);
}

TEST(PieceType, getPieceNum) {
  EXPECT_EQ(Piece(Piece::kEastWind).getPieceNum(), 1);
  EXPECT_EQ(Piece(Piece::kGreenDragon).getPieceNum(), 7);
  EXPECT_EQ(Piece(Piece::kRedFiveCharacter).getPieceNum(), 5);
  EXPECT_EQ(Piece(Piece::kRedFivePin).getPieceNum(), 5);
  EXPECT_EQ(Piece(Piece::kRedFiveBamboo).getPieceNum(), 5);
  EXPECT_EQ(Piece(Piece::kNineCharacter).getPieceNum(), 9);
  EXPECT_EQ(Piece(Piece::kNinePin).getPieceNum(), 9);
  EXPECT_EQ(Piece(Piece::kNineBamboo).getPieceNum(), 9);
  EXPECT_EQ(Piece(Piece::kOneCharacter).getPieceNum(), 1);
  EXPECT_EQ(Piece(Piece::kOnePin).getPieceNum(), 1);
  EXPECT_EQ(Piece(Piece::kOneBamboo).getPieceNum(), 1);
}

TEST(PieceType, isGreen) {
  EXPECT_FALSE(Piece(Piece::kEastWind).isGreen());
  EXPECT_TRUE(Piece(Piece::kGreenDragon).isGreen());
  EXPECT_TRUE(Piece(Piece::kTwoBamboo).isGreen());
  EXPECT_TRUE(Piece(Piece::kThreeBamboo).isGreen());
  EXPECT_TRUE(Piece(Piece::kFourBamboo).isGreen());
  EXPECT_TRUE(Piece(Piece::kSixBamboo).isGreen());
  EXPECT_TRUE(Piece(Piece::kEightBamboo).isGreen());
  EXPECT_FALSE(Piece(Piece::kRedFiveCharacter).isGreen());
  EXPECT_FALSE(Piece(Piece::kRedFivePin).isGreen());
  EXPECT_FALSE(Piece(Piece::kRedFiveBamboo).isGreen());
  EXPECT_FALSE(Piece(Piece::kNineCharacter).isGreen());
  EXPECT_FALSE(Piece(Piece::kNinePin).isGreen());
  EXPECT_FALSE(Piece(Piece::kNineBamboo).isGreen());
  EXPECT_FALSE(Piece(Piece::kOneCharacter).isGreen());
  EXPECT_FALSE(Piece(Piece::kOnePin).isGreen());
  EXPECT_FALSE(Piece(Piece::kOneBamboo).isGreen());
}

TEST(PieceType, fromWind) {
  EXPECT_EQ(Piece::fromWind(kEast), kEastWind);
  EXPECT_EQ(Piece::fromWind(kSouth), kSouthWind);
  EXPECT_EQ(Piece::fromWind(kNorth), kNorthWind);
  EXPECT_EQ(Piece::fromWind(kWest), kWestWind);
}

TEST(PieceType, toStr) {
  EXPECT_EQ(Piece(Piece::kEastWind).toStr(), "East");
  EXPECT_EQ(Piece(-1).toStr(), "ERRORPIECE");
}

TEST(PieceType, operatorPlus) {
  EXPECT_EQ(Piece(Piece::kEastWind) + 1, kError);
  EXPECT_EQ(Piece(Piece::kEastWind) + 0, kEastWind);

  EXPECT_EQ(Piece(Piece::kRedFiveCharacter) + 5, kError);
  EXPECT_EQ(Piece(Piece::kRedFivePin) + 1, kSixPin);
  EXPECT_EQ(Piece(Piece::kRedFiveBamboo) + 4, kNineBamboo);

  EXPECT_EQ(Piece(Piece::kNineCharacter) + 17, kError);
  EXPECT_EQ(Piece(Piece::kNinePin) + 1, kError);

  EXPECT_EQ(Piece(Piece::kOneCharacter) + 1, kTwoCharacter);
  EXPECT_EQ(Piece(Piece::kOnePin) + 2, kThreePin);
  EXPECT_EQ(Piece(Piece::kOneBamboo) + 8, kNineBamboo);
}

TEST(PieceType, operatorMinus) {
  EXPECT_EQ(Piece(Piece::kEastWind) + 0, kEastWind);
  EXPECT_EQ(Piece(Piece::kEastWind) - 1, kError);

  EXPECT_EQ(Piece(Piece::kRedFiveCharacter) - 5, kError);
  EXPECT_EQ(Piece(Piece::kRedFivePin) - 1, kFourPin);
  EXPECT_EQ(Piece(Piece::kRedFiveBamboo) - 4, kOneBamboo);

  EXPECT_EQ(Piece(Piece::kNineCharacter) - 1, kEightCharacter);
  EXPECT_EQ(Piece(Piece::kNinePin) - 2, kSevenPin);
  EXPECT_EQ(Piece(Piece::kNineBamboo) - 8, kOneBamboo);

  EXPECT_EQ(Piece(Piece::kOneCharacter) - 1, kError);
  EXPECT_EQ(Piece(Piece::kOnePin) - 2, kError);
}

TEST(PieceType, operatorEqual) {
  EXPECT_TRUE(Piece(Piece::kEastWind) == Piece(Piece::kEastWind));
  EXPECT_FALSE(Piece(Piece::kEastWind) == Piece(Piece::kSouthWind));
  EXPECT_TRUE(Piece(Piece::kRedFiveBamboo) == Piece(Piece::kFiveBamboo));
  EXPECT_FALSE(Piece(Piece::kRedFiveBamboo) == Piece(Piece::kFivePin));
  EXPECT_TRUE(Piece(Piece::kRedFiveBamboo) == Piece(Piece::kRedFiveBamboo));
}

TEST(PieceType, formPiece) {
  EXPECT_EQ(Piece::formPiece(Piece::kCharacterSuit, 5),
            Piece(Piece::kFiveCharacter));
  EXPECT_EQ(Piece::formPiece(Piece::kPinSuit, 3), Piece(Piece::kThreePin));
  EXPECT_EQ(Piece::formPiece(Piece::kBambooSuit, 1), Piece(Piece::kOneBamboo));
  EXPECT_EQ(Piece::formPiece(Piece::kCharacterSuit, 9),
            Piece(Piece::kNineCharacter));
  EXPECT_EQ(Piece::formPiece(Piece::kHonorSuit, 1), Piece(Piece::kEastWind));
  EXPECT_EQ(Piece::formPiece(Piece::kHonorSuit, 7), Piece(Piece::kGreenDragon));
  EXPECT_EQ(Piece::formPiece(Piece::kCharacterSuit, 5, true),
            Piece(Piece::kRedFiveCharacter));
  EXPECT_EQ(Piece::formPiece(Piece::kHonorSuit, 8), Piece(Piece::kError));
  EXPECT_EQ(Piece::formPiece(Piece::kCharacterSuit, 11), Piece(Piece::kError));
  EXPECT_EQ(Piece::formPiece(Piece::kCharacterSuit, 3, true),
            Piece(Piece::kError));
}

}  // namespace mahjong
