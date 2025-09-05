#include <gtest/gtest.h>

#include <array>

#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/pieces.h"

namespace mahjong {

TEST(RemovePieces, RemoveCorrectAmount) {
  Player player;
  player.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  player.live_count = 4;

  EXPECT_EQ(RemovePieces(player, kOneCharacter, 1), 1);
  EXPECT_EQ(player.live_count, 3);
}

TEST(RemovePieces, RemoveOnlyTheOnesAvailable) {
  Player player;
  player.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  player.live_count = 4;

  EXPECT_EQ(RemovePieces(player, kFourPin, 2), 1);
  EXPECT_EQ(player.live_count, 3);
}

TEST(RemovePieces, OnlyRemovesOne) {
  Player player;
  player.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  player.live_count = 4;

  EXPECT_EQ(RemovePieces(player, kWhiteDragon, 1), 1);
  EXPECT_EQ(player.live[2], kWhiteDragon);
}

TEST(RemovePieces, RemovesZeroWhenPieceDoesntExist) {
  Player player;
  player.live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};
  player.live_count = 4;

  EXPECT_EQ(RemovePieces(player, kGreenDragon, 8), 0);
  EXPECT_EQ(player.live_count, 4);
}

}  // namespace mahjong
