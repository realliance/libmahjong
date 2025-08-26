#include "gamestate_utils.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "controllers/playercontroller.h"
#include "statefunctions/router.h"
#include "statefunctions/statecontroller.h"
#include "types/gamestate.h"
#include "types/settings.h"
#include "types/statefunction.h"
#include "utils/playercontrollerfake.h"

namespace mahjong {

std::unique_ptr<GameState> AdvanceUntilState(std::unique_ptr<GameState> state,
                                             StateFunctionType targetState,
                                             int maxIterations) {
  int iterations = 0;
  while (state->nextState != targetState && iterations < maxIterations &&
         state->nextState != StateFunctionType::kGameEnd &&
         state->nextState != StateFunctionType::kError) {
    state = Router::Instance().Route(state->nextState)(std::move(state));
    iterations++;
  }

  if (iterations >= maxIterations) {
    throw std::runtime_error("AdvanceUntilState: Max iterations reached");
  }

  return state;
}

std::unique_ptr<GameState> AdvanceThroughState(std::unique_ptr<GameState> state,
                                               StateFunctionType targetState,
                                               int maxIterations) {
  // First advance until we reach the target state
  state = AdvanceUntilState(std::move(state), targetState, maxIterations);

  // Then advance once more through that state
  state = AdvanceGameState(std::move(state));

  return state;
}

std::unique_ptr<GameState> CreateTestGameState(uint64_t seed) {
  auto state = std::make_unique<GameState>();
  state->seed = seed;
  for (int i = 0; i < kNumPlayers; i++) {
    state->players[i] = std::make_unique<PlayerControllerFake>();
  }
  state->nextState = StateFunctionType::kGameStart;
  return state;
}

std::unique_ptr<GameState> InitializeTestRound(
    uint64_t seed,
    std::vector<std::unique_ptr<PlayerController>> playerControllers) {
  auto state = std::make_unique<GameState>();
  state->seed = seed;

  // Use provided controllers or default to PlayerControllerFake
  if (!playerControllers.empty()) {
    const size_t num_players =
        std::min(playerControllers.size(), static_cast<size_t>(kNumPlayers));
    for (size_t i = 0; i < num_players; i++) {
      if (playerControllers[i]) {
        state->players[i] = std::move(playerControllers[i]);
      } else {
        state->players[i] = std::make_unique<PlayerControllerFake>();
      }
    }
    // Fill remaining players with default controllers
    for (size_t i = num_players; i < kNumPlayers; i++) {
      state->players[i] = std::make_unique<PlayerControllerFake>();
    }
  } else {
    // All players get default fake controllers
    for (int i = 0; i < kNumPlayers; i++) {
      state->players[i] = std::make_unique<PlayerControllerFake>();
    }
  }

  state->nextState = StateFunctionType::kGameStart;

  // Advance through GameStart and through first draw
  state = AdvanceThroughState(std::move(state), StateFunctionType::kDraw);

  return state;
}

}  // namespace mahjong
