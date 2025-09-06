#include <gtest/gtest.h>

#include <array>

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
  Hand& player = state.players[0];
  player.live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  player.live_count = 3;
  EXPECT_TRUE(CanChi(state, player));
  state.pendingPiece = kTwoCharacter;
  EXPECT_TRUE(CanChi(state, player));
  state.pendingPiece = kThreeCharacter;
  EXPECT_TRUE(CanChi(state, player));
  state.pendingPiece = kFourCharacter;
  EXPECT_TRUE(CanChi(state, player));
}

TEST(CanChi, EastShouldNotChiOnSouth) {
  GameState state;
  state.pendingPiece = kOneCharacter;
  state.currentPlayer = 1;
  Hand& player = state.players[0];
  player.live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  player.live_count = 3;
  EXPECT_FALSE(CanChi(state, player));
}

TEST(CanChi, NoChiOnHonor) {
  GameState state;
  state.pendingPiece = kOneCharacter;
  state.currentPlayer = 3;
  Hand& player = state.players[0];
  player.live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  player.live_count = 3;
  state.pendingPiece = kWhiteDragon;
  EXPECT_FALSE(CanChi(state, player));
}

}  // namespace mahjong
