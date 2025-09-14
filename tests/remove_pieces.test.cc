#include <gtest/gtest.h>

#include <array>

#include "statefunctions/stateutilities.h"
#include "types/hand.h"
#include "types/pieces.h"

namespace mahjong {

TEST(RemovePieces, RemoveCorrectAmount) {
  Hand hand;
  hand.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  hand.live_count = 4;

  EXPECT_EQ(RemovePieces(hand, kOneCharacter, 1), 1);
  EXPECT_EQ(hand.live_count, 3);
}

TEST(RemovePieces, RemoveOnlyTheOnesAvailable) {
  Hand hand;
  hand.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  hand.live_count = 4;

  EXPECT_EQ(RemovePieces(hand, kFourPin, 2), 1);
  EXPECT_EQ(hand.live_count, 3);
}

TEST(RemovePieces, OnlyRemovesOne) {
  Hand hand;
  hand.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  hand.live_count = 4;

  EXPECT_EQ(RemovePieces(hand, kWhiteDragon, 1), 1);
  EXPECT_EQ(hand.live[2], kWhiteDragon);
}

TEST(RemovePieces, RemovesZeroWhenPieceDoesntExist) {
  Hand hand;
  hand.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  hand.live_count = 4;

  EXPECT_EQ(RemovePieces(hand, kGreenDragon, 8), 0);
  EXPECT_EQ(hand.live_count, 4);
}

}  // namespace mahjong
