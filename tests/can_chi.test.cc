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
  Hand& hand = state.hands[0];
  hand.live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  hand.live_count = 3;
  EXPECT_TRUE(CanChi(state, hand));
  state.pendingPiece = kTwoCharacter;
  EXPECT_TRUE(CanChi(state, hand));
  state.pendingPiece = kThreeCharacter;
  EXPECT_TRUE(CanChi(state, hand));
  state.pendingPiece = kFourCharacter;
  EXPECT_TRUE(CanChi(state, hand));
}

TEST(CanChi, EastShouldNotChiOnSouth) {
  GameState state;
  state.pendingPiece = kOneCharacter;
  state.currentPlayer = 1;
  Hand& hand = state.hands[0];
  hand.live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  hand.live_count = 3;
  EXPECT_FALSE(CanChi(state, hand));
}

TEST(CanChi, NoChiOnHonor) {
  GameState state;
  state.pendingPiece = kOneCharacter;
  state.currentPlayer = 3;
  Hand& hand = state.hands[0];
  hand.live = {kOneCharacter, kTwoCharacter, kThreeCharacter};
  hand.live_count = 3;
  state.pendingPiece = kWhiteDragon;
  EXPECT_FALSE(CanChi(state, hand));
}

}  // namespace mahjong
