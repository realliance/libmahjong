#include "gamestate_utils.h"

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "playercontrollerfake.h"
#include "statefunctions/router.h"
#include "statefunctions/statecontroller.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/settings.h"
#include "types/statefunction.h"

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

std::unique_ptr<GameState> CreateTestGameState(uint64_t seed) {
  auto state = std::make_unique<GameState>();
  state->seed = seed;
  for (int i = 0; i < kNumPlayers; i++) {
    state->players[i].controller = std::make_unique<PlayerControllerFake>();
  }
  state->nextState = StateFunctionType::kGameStart;
  return state;
}

std::unique_ptr<GameState> InitializeTestGame(
    uint64_t seed, const std::vector<Event>& playerDecisions) {
  auto state = CreateTestGameState(seed);

  // GameStart
  state = AdvanceGameState(std::move(state));

  // RoundStart
  state = AdvanceGameState(std::move(state));

  // Pre-load each player's fake controller with decisions BEFORE first draw
  // because the draw will trigger player decisions
  if (!playerDecisions.empty()) {
    for (int i = 0; i < kNumPlayers; i++) {
      auto* fake_controller = static_cast<PlayerControllerFake*>(
          state->players[i].controller.get());
      fake_controller->AddEvents(playerDecisions);
    }
  }

  // First Draw
  state = AdvanceGameState(std::move(state));

  return state;
}

std::unique_ptr<GameState> InitializeTestGameToRoundStart(uint64_t seed) {
  auto state = CreateTestGameState(seed);

  // GameStart
  state =
      Router::Instance().Route(StateFunctionType::kGameStart)(std::move(state));

  // RoundStart
  state = Router::Instance().Route(StateFunctionType::kRoundStart)(
      std::move(state));

  return state;
}

}  // namespace mahjong
