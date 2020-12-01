#include <gtest/gtest.h>
#include "piecetype.h"

using namespace Mahjong;

TEST(PieceType, toUint8_t) {
  EXPECT_EQ(Piece(Piece::EAST_WIND).toUint8_t(),1);
  EXPECT_EQ(Piece(Piece::GREEN_DRAGON).toUint8_t(),7);
  EXPECT_EQ(Piece(Piece::RED_FIVE_CHARACTER).toUint8_t(),101);
  EXPECT_EQ(Piece(Piece::RED_FIVE_PIN).toUint8_t(),69);
  EXPECT_EQ(Piece(Piece::RED_FIVE_BAMBOO).toUint8_t(),37);
  EXPECT_EQ(Piece(Piece::NINE_CHARACTER).toUint8_t(),233);
  EXPECT_EQ(Piece(Piece::NINE_PIN).toUint8_t(),201);
  EXPECT_EQ(Piece(Piece::NINE_BAMBOO).toUint8_t(),169);
  EXPECT_EQ(Piece(Piece::ONE_CHARACTER).toUint8_t(),225);
  EXPECT_EQ(Piece(Piece::ONE_PIN).toUint8_t(),193);
  EXPECT_EQ(Piece(Piece::ONE_BAMBOO).toUint8_t(),161);
}

TEST(PieceType, raw_value){
  EXPECT_EQ(Piece(Piece::EAST_WIND).raw_value(),1);
  EXPECT_EQ(Piece(Piece::GREEN_DRAGON).raw_value(),7);
  EXPECT_EQ(Piece(Piece::RED_FIVE_CHARACTER).raw_value(),117);
  EXPECT_EQ(Piece(Piece::RED_FIVE_PIN).raw_value(),85);
  EXPECT_EQ(Piece(Piece::RED_FIVE_BAMBOO).raw_value(),53);
  EXPECT_EQ(Piece(Piece::NINE_CHARACTER).raw_value(),233);
  EXPECT_EQ(Piece(Piece::NINE_PIN).raw_value(),201);
  EXPECT_EQ(Piece(Piece::NINE_BAMBOO).raw_value(),169);
  EXPECT_EQ(Piece(Piece::ONE_CHARACTER).raw_value(),225);
  EXPECT_EQ(Piece(Piece::ONE_PIN).raw_value(),193);
  EXPECT_EQ(Piece(Piece::ONE_BAMBOO).raw_value(),161);
}

TEST(PieceType, isHonor){
  EXPECT_TRUE(Piece(Piece::EAST_WIND).isHonor());
  EXPECT_TRUE(Piece(Piece::GREEN_DRAGON).isHonor());
  EXPECT_FALSE(Piece(Piece::RED_FIVE_CHARACTER).isHonor());
  EXPECT_FALSE(Piece(Piece::RED_FIVE_PIN).isHonor());
  EXPECT_FALSE(Piece(Piece::RED_FIVE_BAMBOO).isHonor());
  EXPECT_FALSE(Piece(Piece::NINE_CHARACTER).isHonor());
  EXPECT_FALSE(Piece(Piece::NINE_PIN).isHonor());
  EXPECT_FALSE(Piece(Piece::NINE_BAMBOO).isHonor());
  EXPECT_FALSE(Piece(Piece::ONE_CHARACTER).isHonor());
  EXPECT_FALSE(Piece(Piece::ONE_PIN).isHonor());
  EXPECT_FALSE(Piece(Piece::ONE_BAMBOO).isHonor());
}

TEST(PieceType, isTerminal){
  EXPECT_FALSE(Piece(Piece::EAST_WIND).isTerminal());
  EXPECT_FALSE(Piece(Piece::GREEN_DRAGON).isTerminal());
  EXPECT_FALSE(Piece(Piece::RED_FIVE_CHARACTER).isTerminal());
  EXPECT_FALSE(Piece(Piece::RED_FIVE_PIN).isTerminal());
  EXPECT_FALSE(Piece(Piece::RED_FIVE_BAMBOO).isTerminal());
  EXPECT_TRUE(Piece(Piece::NINE_CHARACTER).isTerminal());
  EXPECT_TRUE(Piece(Piece::NINE_PIN).isTerminal());
  EXPECT_TRUE(Piece(Piece::NINE_BAMBOO).isTerminal());
  EXPECT_TRUE(Piece(Piece::ONE_CHARACTER).isTerminal());
  EXPECT_TRUE(Piece(Piece::ONE_PIN).isTerminal());
  EXPECT_TRUE(Piece(Piece::ONE_BAMBOO).isTerminal());
}

TEST(PieceType, getSuit){
  EXPECT_EQ(Piece(Piece::EAST_WIND).getSuit(),Piece::HONOR_SUIT);
  EXPECT_EQ(Piece(Piece::GREEN_DRAGON).getSuit(),Piece::HONOR_SUIT);
  EXPECT_EQ(Piece(Piece::RED_FIVE_CHARACTER).getSuit(),Piece::CHARACTER_SUIT);
  EXPECT_EQ(Piece(Piece::RED_FIVE_PIN).getSuit(),Piece::PIN_SUIT);
  EXPECT_EQ(Piece(Piece::RED_FIVE_BAMBOO).getSuit(),Piece::BAMBOO_SUIT);
  EXPECT_EQ(Piece(Piece::NINE_CHARACTER).getSuit(),Piece::CHARACTER_SUIT);
  EXPECT_EQ(Piece(Piece::NINE_PIN).getSuit(),Piece::PIN_SUIT);
  EXPECT_EQ(Piece(Piece::NINE_BAMBOO).getSuit(),Piece::BAMBOO_SUIT);
  EXPECT_EQ(Piece(Piece::ONE_CHARACTER).getSuit(),Piece::CHARACTER_SUIT);
  EXPECT_EQ(Piece(Piece::ONE_PIN).getSuit(),Piece::PIN_SUIT);
  EXPECT_EQ(Piece(Piece::ONE_BAMBOO).getSuit(),Piece::BAMBOO_SUIT);
}

TEST(PieceType, getPieceNum){
  EXPECT_EQ(Piece(Piece::EAST_WIND).getPieceNum(),1);
  EXPECT_EQ(Piece(Piece::GREEN_DRAGON).getPieceNum(),7);
  EXPECT_EQ(Piece(Piece::RED_FIVE_CHARACTER).getPieceNum(),5);
  EXPECT_EQ(Piece(Piece::RED_FIVE_PIN).getPieceNum(),5);
  EXPECT_EQ(Piece(Piece::RED_FIVE_BAMBOO).getPieceNum(),5);
  EXPECT_EQ(Piece(Piece::NINE_CHARACTER).getPieceNum(),9);
  EXPECT_EQ(Piece(Piece::NINE_PIN).getPieceNum(),9);
  EXPECT_EQ(Piece(Piece::NINE_BAMBOO).getPieceNum(),9);
  EXPECT_EQ(Piece(Piece::ONE_CHARACTER).getPieceNum(),1);
  EXPECT_EQ(Piece(Piece::ONE_PIN).getPieceNum(),1);
  EXPECT_EQ(Piece(Piece::ONE_BAMBOO).getPieceNum(),1);
}

TEST(PieceType, isGreen){
  EXPECT_FALSE(Piece(Piece::EAST_WIND).isGreen());
  EXPECT_TRUE(Piece(Piece::GREEN_DRAGON).isGreen());
  EXPECT_TRUE(Piece(Piece::TWO_BAMBOO).isGreen());
  EXPECT_TRUE(Piece(Piece::THREE_BAMBOO).isGreen());
  EXPECT_TRUE(Piece(Piece::FOUR_BAMBOO).isGreen());
  EXPECT_TRUE(Piece(Piece::SIX_BAMBOO).isGreen());
  EXPECT_TRUE(Piece(Piece::EIGHT_BAMBOO).isGreen());
  EXPECT_FALSE(Piece(Piece::RED_FIVE_CHARACTER).isGreen());
  EXPECT_FALSE(Piece(Piece::RED_FIVE_PIN).isGreen());
  EXPECT_FALSE(Piece(Piece::RED_FIVE_BAMBOO).isGreen());
  EXPECT_FALSE(Piece(Piece::NINE_CHARACTER).isGreen());
  EXPECT_FALSE(Piece(Piece::NINE_PIN).isGreen());
  EXPECT_FALSE(Piece(Piece::NINE_BAMBOO).isGreen());
  EXPECT_FALSE(Piece(Piece::ONE_CHARACTER).isGreen());
  EXPECT_FALSE(Piece(Piece::ONE_PIN).isGreen());
  EXPECT_FALSE(Piece(Piece::ONE_BAMBOO).isGreen());
}

TEST(PieceType, fromWind){
  EXPECT_EQ(Piece::fromWind(East),Piece::EAST_WIND);
  EXPECT_EQ(Piece::fromWind(South),Piece::SOUTH_WIND);
  EXPECT_EQ(Piece::fromWind(North),Piece::NORTH_WIND);
  EXPECT_EQ(Piece::fromWind(West),Piece::WEST_WIND);
}

TEST(PieceType, toStr){
  EXPECT_EQ(Piece(Piece::EAST_WIND).toStr(),"East");
  EXPECT_EQ(Piece(-1).toStr(),"ERRORPIECE"); 
}

TEST(PieceType, operatorPlus){
  EXPECT_EQ(Piece(Piece::EAST_WIND)+1,Piece::ERROR);
  EXPECT_EQ(Piece(Piece::EAST_WIND)+0,Piece::EAST_WIND);

  EXPECT_EQ(Piece(Piece::RED_FIVE_CHARACTER)+5,Piece::ERROR);
  EXPECT_EQ(Piece(Piece::RED_FIVE_PIN)+1,Piece::SIX_PIN);
  EXPECT_EQ(Piece(Piece::RED_FIVE_BAMBOO)+4,Piece::NINE_BAMBOO);

  EXPECT_EQ(Piece(Piece::NINE_CHARACTER)+17,Piece::ERROR);
  EXPECT_EQ(Piece(Piece::NINE_PIN)+1,Piece::ERROR);

  EXPECT_EQ(Piece(Piece::ONE_CHARACTER)+1,Piece::TWO_CHARACTER);
  EXPECT_EQ(Piece(Piece::ONE_PIN)+2,Piece::THREE_PIN);
  EXPECT_EQ(Piece(Piece::ONE_BAMBOO)+8,Piece::NINE_BAMBOO);
}

TEST(PieceType, operatorMinus){
  EXPECT_EQ(Piece(Piece::EAST_WIND)+0,Piece::EAST_WIND);
  EXPECT_EQ(Piece(Piece::EAST_WIND)-1,Piece::ERROR);

  EXPECT_EQ(Piece(Piece::RED_FIVE_CHARACTER)-5,Piece::ERROR);
  EXPECT_EQ(Piece(Piece::RED_FIVE_PIN)-1,Piece::FOUR_PIN);
  EXPECT_EQ(Piece(Piece::RED_FIVE_BAMBOO)-4,Piece::ONE_BAMBOO);

  EXPECT_EQ(Piece(Piece::NINE_CHARACTER)-1,Piece::EIGHT_CHARACTER);
  EXPECT_EQ(Piece(Piece::NINE_PIN)-2,Piece::SEVEN_PIN);
  EXPECT_EQ(Piece(Piece::NINE_BAMBOO)-8,Piece::ONE_BAMBOO);

  EXPECT_EQ(Piece(Piece::ONE_CHARACTER)-1,Piece::ERROR);
  EXPECT_EQ(Piece(Piece::ONE_PIN)-2,Piece::ERROR);
}

TEST(PieceType, operatorEqual){
  EXPECT_TRUE(Piece(Piece::EAST_WIND)==Piece(Piece::EAST_WIND));
  EXPECT_FALSE(Piece(Piece::EAST_WIND)==Piece(Piece::SOUTH_WIND));
  EXPECT_TRUE(Piece(Piece::RED_FIVE_BAMBOO)==Piece(Piece::FIVE_BAMBOO));
  EXPECT_FALSE(Piece(Piece::RED_FIVE_BAMBOO)==Piece(Piece::FIVE_PIN));
  EXPECT_TRUE(Piece(Piece::RED_FIVE_BAMBOO)==Piece(Piece::RED_FIVE_BAMBOO));
}
