#include "gamestate_utils.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>

#include "types/gamestate.h"
#include "types/settings.h"
#include "types/statefunction.h"

namespace mahjong {

TEST(GameStateUtils, AdvanceUntilState) {
  auto state = CreateTestGameState();
  state = AdvanceUntilState(std::move(state), StateFunctionType::kRoundStart);
  EXPECT_EQ(state->nextState, StateFunctionType::kRoundStart);
}

TEST(GameStateUtils, AdvanceUntilStateThrowOnMaxIterations) {
  auto state = CreateTestGameState();
  // Won't reach this in one iteration
  EXPECT_THROW(AdvanceUntilState(std::move(state), StateFunctionType::kDraw, 1),
               std::runtime_error);
}

TEST(GameStateUtils, AdvanceThroughState) {
  auto state = CreateTestGameState();
  state = AdvanceThroughState(std::move(state), StateFunctionType::kGameStart);
  // Should have executed it and moved to the next state
  EXPECT_EQ(state->currState, StateFunctionType::kGameStart);
  EXPECT_EQ(state->nextState, StateFunctionType::kRoundStart);
}

TEST(GameStateUtils, InitializeTestRoundDefault) {
  const uint64_t test_seed = 1337;
  auto state = InitializeTestRound(test_seed);

  EXPECT_EQ(state->seed, test_seed);
  // Should have advanced through Draw
  EXPECT_EQ(state->currState, StateFunctionType::kDraw);

  // All players are filled with a controller
  for (int i = 0; i < kNumPlayers; i++) {
    EXPECT_NE(state->controllers[i], nullptr);
  }

  // Draw occurred
  EXPECT_EQ(state->players[0].live_count, 14);

  for (int i = 1; i < kNumPlayers; i++) {
    EXPECT_EQ(state->players[i].live_count, 13);
  }
}

}  // namespace mahjong
