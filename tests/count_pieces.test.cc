#include <gtest/gtest.h>

#include <array>

#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/pieces.h"

namespace mahjong {

TEST(CountPieces, CountPieces) {
  Player player;
  player.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  player.live_count = 4;

  EXPECT_EQ(CountPieces(player, kOneCharacter), 1);
  EXPECT_EQ(CountPieces(player, kFourPin), 1);
  EXPECT_EQ(CountPieces(player, kWhiteDragon), 2);
  EXPECT_EQ(CountPieces(player, kGreenDragon), 0);
}

}  // namespace mahjong
