#include <gtest/gtest.h>
#include <array>

#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong {

TEST(CountPieces, CountPieces) {
  GameState state;
  state.hands[0].live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};

  EXPECT_EQ(CountPieces(state, 0, kOneCharacter), 1);
  EXPECT_EQ(CountPieces(state, 0, kFourPin), 1);
  EXPECT_EQ(CountPieces(state, 0, kWhiteDragon), 2);
  EXPECT_EQ(CountPieces(state, 0, kGreenDragon), 0);
}

}  // namespace mahjong
