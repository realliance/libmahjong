#include <gtest/gtest.h>

#include <memory>
#include <utility>
#include <vector>

#include "statefunctions/statecontroller.h"
#include "types/gamestate.h"
#include "types/settings.h"
#include "types/statefunction.h"
#include "types/walls.h"
#include "utils/gamestate_utils.h"

namespace mahjong {

TEST(GameSetup, playerCount) {
  auto state = CreateTestGameState();
  state = AdvanceGameState(std::move(state));
  // We just ran game start
  EXPECT_EQ(state->currState, StateFunctionType::kGameStart);
  // Number of players is as expected
  EXPECT_EQ(state->players.size(), kNumPlayers);
}

TEST(GameSetup, playerScore) {
  auto state = CreateTestGameState();
  state = AdvanceGameState(std::move(state));
  // We just ran game start
  EXPECT_EQ(state->currState, StateFunctionType::kGameStart);
  // All players have the expected starting points
  for (int i = 0; i < kNumPlayers; i++) {
    EXPECT_EQ(state->players[i].points, kStartingPoints);
  }
}

TEST(RoundSetup, playerPlayers) {
  auto state = CreateTestGameState();
  // Advance through Round Start
  state = AdvanceThroughState(std::move(state), StateFunctionType::kRoundStart);

  // Each player should have 13 tiles
  for (int i = 0; i < kNumPlayers; i++) {
    EXPECT_EQ(state->players[i].live_count, 13)
        << "Player " << i << " should have 13 tiles";
  }
}

TEST(RoundSetup, wall) {
  auto state = CreateTestGameState(12345);
  // Advance through round start
  state = AdvanceThroughState(std::move(state), StateFunctionType::kRoundStart);

  // Verify the seed was used
  EXPECT_EQ(state->seed, 12345);

  // Check that walls were initialized correctly
  EXPECT_EQ(Walls::GetRemainingPieces(*state), 70);
  EXPECT_EQ(state->deadWallIndex, 0);

  // Check that dora indicator is set
  EXPECT_EQ(Walls::GetDoras(*state).size(), 1);
}

TEST(TurnOrder, initialPlayer) {
  auto state = CreateTestGameState();
  // Advance through round start
  state = AdvanceThroughState(std::move(state), StateFunctionType::kRoundStart);

  // Before first draw, currentPlayer should be -1
  EXPECT_EQ(state->currentPlayer, -1);

  // After first draw, player 0 (East) should start
  state = AdvanceGameState(std::move(state));
  EXPECT_EQ(state->currState, StateFunctionType::kDraw);
  EXPECT_EQ(state->currentPlayer, 0);
  EXPECT_EQ(state->turnNum, 0);
}

TEST(TurnOrder, playerRotation) {
  auto state = InitializeTestRound();

  // Simulate multiple turns to verify rotation
  for (int turn = 0; turn < 8; turn++) {
    const int expected_player = turn % kNumPlayers;
    EXPECT_EQ(state->currentPlayer, expected_player) << "Turn " << turn;
    EXPECT_EQ(state->turnNum, turn);

    state = AdvanceGameState(std::move(state));
    EXPECT_EQ(state->currState, StateFunctionType::kPlayerPlayer);
    state = AdvanceGameState(std::move(state));
    EXPECT_EQ(state->currState, StateFunctionType::kDiscard);
    state = AdvanceGameState(std::move(state));
    EXPECT_EQ(state->currState, StateFunctionType::kDraw);
  }
}

TEST(DrawMechanics, drawIncreasesTileCount) {
  auto state = CreateTestGameState();
  // Advance through round start
  state = AdvanceThroughState(std::move(state), StateFunctionType::kRoundStart);

  // All players start with 13 tiles
  for (int i = 0; i < kNumPlayers; i++) {
    EXPECT_EQ(state->players[i].live_count, 13);
  }

  const int walls_before = Walls::GetRemainingPieces(*state);

  // First player draws
  state = AdvanceGameState(std::move(state));
  EXPECT_EQ(state->currState, StateFunctionType::kDraw);

  // Player 0 should now have 14 tiles
  EXPECT_EQ(state->players[0].live_count, 14);

  // Other players still have 13
  for (int i = 1; i < kNumPlayers; i++) {
    EXPECT_EQ(state->players[i].live_count, 13);
  }

  // Wall should have one less tile
  EXPECT_EQ(Walls::GetRemainingPieces(*state), walls_before - 1);
}

TEST(StateMachine, earlyGameTransitions) {
  auto state = CreateTestGameState();
  // Advance through Round Start
  state = AdvanceThroughState(std::move(state), StateFunctionType::kRoundStart);

  // Verify early game transitions (RoundStart -> Draw -> PlayerPlayer)
  // After round start, next state should be draw
  EXPECT_EQ(state->nextState, StateFunctionType::kDraw);
  state = AdvanceGameState(std::move(state));
  EXPECT_EQ(state->currState, StateFunctionType::kDraw);

  // After draw, next state should be player hand
  EXPECT_EQ(state->nextState, StateFunctionType::kPlayerPlayer);
}

TEST(DiscardMechanics, discardDecreasesTileCount) {
  auto state = InitializeTestRound();
  EXPECT_EQ(state->currState, StateFunctionType::kDraw);

  // Player 0 has 14 tiles after draw
  EXPECT_EQ(state->players[0].live_count, 14);

  // Move through player hand to discard
  state = AdvanceGameState(std::move(state));
  EXPECT_EQ(state->currState, StateFunctionType::kPlayerPlayer);
  EXPECT_EQ(state->nextState, StateFunctionType::kDiscard);

  const int discards_before = state->players[0].discards_count;
  state = AdvanceGameState(std::move(state));
  EXPECT_EQ(state->currState, StateFunctionType::kDiscard);

  // Player should be back to 13 tiles
  EXPECT_EQ(state->players[0].live_count, 13);

  // Discard pile should have increased
  EXPECT_EQ(state->players[0].discards_count, discards_before + 1);
}

}  // namespace mahjong
