#pragma once
#include <memory>
#include <optional>
#include <vector>

#include "statefunctions/router.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/statefunction.h"

namespace mahjong {

// Advances game state until it reaches the target state or max iterations
std::unique_ptr<GameState> AdvanceUntilState(std::unique_ptr<GameState> state,
                                             StateFunctionType targetState,
                                             int maxIterations = 100);

// Helper to create a standard test game state with fake controllers
std::unique_ptr<GameState> CreateTestGameState(uint64_t seed = 12345);

// Advances through standard game initialization (GameStart -> RoundStart -> Draw)
// Optionally pre-loads each player's fake controller with the given decisions
std::unique_ptr<GameState> InitializeTestGame(
    uint64_t seed = 12345, const std::vector<Event>& playerDecisions = {});

// Debug version that stops at RoundStart to isolate issues
std::unique_ptr<GameState> InitializeTestGameToRoundStart(
    uint64_t seed = 12345);

}  // namespace mahjong
