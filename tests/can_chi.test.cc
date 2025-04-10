#include <gtest/gtest.h>
#include <array>
#include <string>
#include <vector>

#include "statefunctions/decisionfunction.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong {

TEST(CanChi, EastShouldChiOnNorth) {
  GameState state;
  state.pendingPiece = kOneCharacter;
  state.currentPlayer = 3;
  state.hands[0].live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  EXPECT_TRUE(CanChi(state, 0));
  state.pendingPiece = kTwoCharacter;
  EXPECT_TRUE(CanChi(state, 0));
  state.pendingPiece = kThreeCharacter;
  EXPECT_TRUE(CanChi(state, 0));
  state.pendingPiece = kFourCharacter;
  EXPECT_TRUE(CanChi(state, 0));
}

TEST(CanChi, EastShouldNotChiOnSouth) {
  GameState state;
  state.pendingPiece = kOneCharacter;
  state.currentPlayer = 1;
  state.hands[0].live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  EXPECT_FALSE(CanChi(state, 0));
}

TEST(CanChi, NoChiOnHonor) {
  GameState state;
  state.pendingPiece = kOneCharacter;
  state.currentPlayer = 3;
  state.hands[0].live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  state.pendingPiece = kWhiteDragon;
  EXPECT_FALSE(CanChi(state, 0));
}

}  // namespace mahjong
