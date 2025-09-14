#include <gtest/gtest.h>

#include <array>

#include "statefunctions/stateutilities.h"
#include "types/hand.h"
#include "types/pieces.h"

namespace mahjong {

TEST(CountPieces, CountPieces) {
  Hand hand;
  hand.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  hand.live_count = 4;

  EXPECT_EQ(CountPieces(hand, kOneCharacter), 1);
  EXPECT_EQ(CountPieces(hand, kFourPin), 1);
  EXPECT_EQ(CountPieces(hand, kWhiteDragon), 2);
  EXPECT_EQ(CountPieces(hand, kGreenDragon), 0);
}

}  // namespace mahjong
