#include <gtest/gtest.h>


#include "statefunctions/stateutilities.h"
#include "types/pieces.h"
#include "types/hand.h"

namespace mahjong {

TEST(DiscardPiece, DiscardsCorrectPiece) {
  Hand player;
  player.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  player.live_count = 4;

  DiscardPiece(player, kOneCharacter);
  EXPECT_EQ(player.discards[0], kOneCharacter);
}

TEST(DiscardPiece, DoesntDiscardInCorrectPiece) {
  Hand player;
  player.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  player.live_count = 4;

  DiscardPiece(player, kGreenDragon);
  EXPECT_EQ(player.live_count, 4);
}

}  // namespace mahjong
