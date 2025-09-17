#include <gtest/gtest.h>

#include "statefunctions/stateutilities.h"
#include "types/hand.h"
#include "types/pieces.h"

namespace mahjong {

TEST(DiscardPiece, DiscardsCorrectPiece) {
  Hand hand;
  hand.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  hand.live_count = 4;

  DiscardPiece(hand, kOneCharacter);
  EXPECT_EQ(hand.discards[0], kOneCharacter);
}

TEST(DiscardPiece, DoesntDiscardInCorrectPiece) {
  Hand hand;
  hand.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  hand.live_count = 4;

  DiscardPiece(hand, kGreenDragon);
  EXPECT_EQ(hand.live_count, 4);
}

}  // namespace mahjong
