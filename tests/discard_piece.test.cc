#include <gtest/gtest.h>

#include <array>

#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/pieces.h"

namespace mahjong {

TEST(DiscardPiece, DiscardsCorrectPiece) {
    Player  player;
  player.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  player.live_count = 4;

  DiscardPiece(player, kOneCharacter);
  EXPECT_EQ(player.discards[0], kOneCharacter);
}

TEST(DiscardPiece, DoesntDiscardInCorrectPiece) {
  Player  player;
  player.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  player.live_count = 4;

  DiscardPiece(player, kGreenDragon);
  EXPECT_EQ(player.live_count, 4);
}

}  // namespace mahjong
