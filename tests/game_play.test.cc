#include <gtest/gtest.h>
#include <array>
#include <vector>

#include "analysis/hands.h"
#include "statefunctions/decisionfunction.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong {

TEST(GamePlay, Furiten) {
  GameState state;
  state.pendingPiece = kRedDragon;
  state.hands[0].live = {kRedDragon, kRedDragon, kRedDragon};
  ASSERT_TRUE(isComplete(state, 0));
  // Place Red Dragon in Discard Pile
  state.hands[0].discards = {kRedDragon};
  state.hands[0].live = {kRedDragon, kRedDragon};
  EXPECT_FALSE(CanRon(state, 0));
}

}  // namespace mahjong
