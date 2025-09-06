#pragma once
#include <memory>

#include "controllers/playercontroller.h"
#include "types/gamestate.h"
#include "types/settings.h"
#include "types/statefunction.h"

namespace mahjong {

// Advances game state until it reaches the target state or max iterations
std::unique_ptr<GameState> AdvanceUntilState(std::unique_ptr<GameState> state,
                                             StateFunctionType targetState,
                                             int maxIterations = 100);

// Advances game state until it reaches the target state, then advances once
// more
std::unique_ptr<GameState> AdvanceThroughState(std::unique_ptr<GameState> state,
                                               StateFunctionType targetState,
                                               int maxIterations = 100);

// Helper to create a standard test game state with fake controllers
std::unique_ptr<GameState> CreateTestGameState(uint64_t seed = 12345);

// Advances through standard round initialization
std::unique_ptr<GameState> InitializeTestRound(
    uint64_t seed = 12345,
    std::array<std::unique_ptr<PlayerController>, kNumPlayers>
        playerControllers = {});

}  // namespace mahjong
