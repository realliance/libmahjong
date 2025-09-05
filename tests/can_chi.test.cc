#include <gtest/gtest.h>

#include <array>

#include "statefunctions/decisionfunction.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong {

TEST(CanChi, EastShouldChiOnNorth) {
  GameState state;
  state.pendingPiece = kOneCharacter;
  state.currentPlayer = 3;
  state.players[0].live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  state.players[0].live_count = 3;
  EXPECT_TRUE(CanChi(state, state.players[0]));
  state.pendingPiece = kTwoCharacter;
  EXPECT_TRUE(CanChi(state, state.players[0]));
  state.pendingPiece = kThreeCharacter;
  EXPECT_TRUE(CanChi(state, state.players[0]));
  state.pendingPiece = kFourCharacter;
  EXPECT_TRUE(CanChi(state, state.players[0]));
}

TEST(CanChi, EastShouldNotChiOnSouth) {
  GameState state;
  state.pendingPiece = kOneCharacter;
  state.currentPlayer = 1;
  state.players[0].live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  state.players[0].live_count = 3;
  EXPECT_FALSE(CanChi(state, state.players[0]));
}

TEST(CanChi, NoChiOnHonor) {
  GameState state;
  state.pendingPiece = kOneCharacter;
  state.currentPlayer = 3;
  state.players[0].live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  state.players[0].live_count = 3;
  state.pendingPiece = kWhiteDragon;
  EXPECT_FALSE(CanChi(state, state.players[0]));
}

}  // namespace mahjong
