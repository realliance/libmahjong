#include <gtest/gtest.h>
#include <array>
#include <string>
#include <vector>

#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "statefunctions/stateutilities.h"
#include "types/hand.h"

namespace mahjong {

TEST(RemovePieces, RemoveCorrectAmount) {
  GameState state;
  state.hands[0].live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};

  EXPECT_EQ(RemovePieces(state, 0, kOneCharacter, 1), 1);
  EXPECT_EQ(state.hands[0].live.size(), 3);
}

TEST(RemovePieces, RemoveOnlyTheOnesAvailable) {
  GameState state;
  state.hands[0].live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};

  EXPECT_EQ(RemovePieces(state, 0, kFourPin, 2), 1);
  EXPECT_EQ(state.hands[0].live.size(), 3);
}

TEST(RemovePieces, OnlyRemovesOne) {
  GameState state;
  state.hands[0].live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};

  EXPECT_EQ(RemovePieces(state, 0, kWhiteDragon, 1), 1);
  EXPECT_EQ(state.hands[0].live.back(), kWhiteDragon);
}

TEST(RemovePieces, RemovesZeroWhenPieceDoesntExist) {
  GameState state;
  state.hands[0].live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};

  EXPECT_EQ(RemovePieces(state, 0, kGreenDragon, 8), 0);
  EXPECT_EQ(state.hands[0].live.size(), 4);
}

}  // namespace mahjong
