#include <gtest/gtest.h>

#include <array>

#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong {

TEST(DiscardPiece, DiscardsCorrectPiece) {
  GameState state;
  state.hands[0].live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};

  DiscardPiece(state, 0, kOneCharacter);
  EXPECT_EQ(state.hands[0].discards[0], kOneCharacter);
}

TEST(DiscardPiece, DoesntDiscardInCorrectPiece) {
  GameState state;
  state.hands[0].live = {kOneCharacter, kFourPin, kWhiteDragon, kWhiteDragon};

  DiscardPiece(state, 0, kGreenDragon);
  EXPECT_EQ(state.hands[0].live.size(), 4);
}

}  // namespace mahjong
