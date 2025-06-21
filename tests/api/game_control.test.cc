#include <gtest/gtest.h>
#include <cstdint>
#include <cstring>
#include "api/gamestate.h"
#include "api/types.h"
#include "types/gamestate.h"

namespace {

const api::CGameSettings kDefaultSettings{
    .seed = 12345,
    .seat_controllers = {"AngryDiscardoBot", "AngryDiscardoBot",
                         "AngryDiscardoBot", "AngryDiscardoBot"},
    .num_controllers = 4,
};

TEST(Api, InitGameState) {
  const api::CGameSettings settings = kDefaultSettings;

  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state, nullptr);
}

TEST(Api, SettingsConversion) {
  const api::CGameSettings settings = kDefaultSettings;

  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state, nullptr);
  EXPECT_EQ(state->seed, settings.seed);
  EXPECT_EQ(state->players.size(), settings.num_controllers);
  for (uint64_t i = 0; i < state->players.size(); ++i) {
    EXPECT_EQ(state->players[i].controller->Name(),
              settings.seat_controllers[i]);
  }
}

TEST(Api, AdvanceGameState) {
  const api::CGameSettings settings = kDefaultSettings;

  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state->seed, 0);

  mahjong::GameState* new_state = api::AdvanceGameState(state);
  EXPECT_NE(new_state, nullptr);
}

TEST(Api, ObserveGameState) {
  const api::CGameSettings settings = kDefaultSettings;
  
  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state, nullptr);
  
  api::CObservedGameState observed = api::ObserveGameState(state);
  
  // Check basic fields are copied
  EXPECT_EQ(observed.currentPlayer, state->currentPlayer);
  EXPECT_EQ(observed.turnNum, state->turnNum);
  EXPECT_EQ(observed.roundNum, state->roundNum);
  EXPECT_EQ(observed.riichiSticks, state->riichiSticks);
  EXPECT_EQ(observed.counters, state->counters);
  EXPECT_EQ(observed.lastCall, state->lastCall);
  EXPECT_EQ(observed.lastCaller, state->lastCaller);
  EXPECT_EQ(observed.concealedKan, state->concealedKan);
  EXPECT_EQ(observed.seed, state->seed);
  
  // Check pending piece conversion
  EXPECT_EQ(observed.pendingPiece, static_cast<api::CPiece>(state->pendingPiece.toUint8_t()));
  
  // Check state function names
  EXPECT_NE(observed.prevState, nullptr);
  EXPECT_NE(observed.currState, nullptr);
  EXPECT_NE(observed.nextState, nullptr);
  EXPECT_STREQ(observed.nextState, "GameStart");  // Initial state should be GameStart
  
  // Check arrays are properly sized and initialized
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(observed.scores[i], state->scores[i]);
    EXPECT_EQ(observed.points[i], state->players[i].points);
    EXPECT_EQ(observed.hasRonned[i], state->hasRonned[i]);
  }
  
  // Clean up allocated memory
  api::FreeObservedGameState(&observed);
  api::FreeGameState(state);
}

TEST(Api, ObserveGameStateAfterAdvancement) {
  const api::CGameSettings settings = kDefaultSettings;
  
  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state, nullptr);
  
  // Advance the game state a few times
  state = api::AdvanceGameState(state);
  EXPECT_NE(state, nullptr);
  
  state = api::AdvanceGameState(state);
  EXPECT_NE(state, nullptr);
  
  api::CObservedGameState observed = api::ObserveGameState(state);
  
  // Verify the observation reflects the advanced state
  EXPECT_EQ(observed.currentPlayer, state->currentPlayer);
  EXPECT_EQ(observed.turnNum, state->turnNum);
  EXPECT_EQ(observed.roundNum, state->roundNum);
  EXPECT_EQ(observed.seed, state->seed);
  
  // After advancement, some fields should have changed from initial values
  // (exact values depend on game logic, but we can check they're copied correctly)
  EXPECT_EQ(observed.riichiSticks, state->riichiSticks);
  EXPECT_EQ(observed.counters, state->counters);
  
  // Check state function names are valid
  EXPECT_NE(observed.prevState, nullptr);
  EXPECT_NE(observed.currState, nullptr);
  EXPECT_NE(observed.nextState, nullptr);

  // Check points are copied
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(observed.points[i], state->players[i].points);
  }
  
  // Clean up allocated memory
  api::FreeObservedGameState(&observed);
  api::FreeGameState(state);
}

TEST(Api, ObserveGameStateHandsAndDiscards) {
  const api::CGameSettings settings = kDefaultSettings;
  
  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state, nullptr);
  
  // Advance to get some hands dealt
  state = api::AdvanceGameState(state);
  EXPECT_NE(state, nullptr);
  
  api::CObservedGameState observed = api::ObserveGameState(state);
  
  // Check that hands are copied (after round start, players should have hands)
  for (int player = 0; player < 4; player++) {
    // Verify hand pieces are copied correctly
    for (int piece = 0; piece < 14; piece++) {
      if (piece < static_cast<int>(state->hands[player].live.size())) {
        EXPECT_EQ(observed.hands[player][piece], 
                  static_cast<api::CPiece>(state->hands[player].live[piece].toUint8_t()));
      } else {
        // Should be filled with error pieces
        EXPECT_EQ(observed.hands[player][piece], 
                  static_cast<api::CPiece>(mahjong::Piece::Type::kError));
      }
    }
    
    // Check discards using pointer arrays
    EXPECT_EQ(observed.playerDiscardCounts[player], static_cast<int>(state->hands[player].discards.size()));
    
    if (state->hands[player].discards.empty()) {
      EXPECT_EQ(observed.players[player], nullptr);
    } else {
      EXPECT_NE(observed.players[player], nullptr);
      // Verify each discard is copied correctly
      for (int discard = 0; discard < observed.playerDiscardCounts[player]; discard++) {
        EXPECT_EQ(observed.players[player][discard],
                  static_cast<api::CPiece>(state->hands[player].discards[discard].toUint8_t()));
      }
    }
  }
  
  // Clean up allocated memory
  api::FreeObservedGameState(&observed);
  api::FreeGameState(state);
}

TEST(Api, FreeObservedGameState) {
  const api::CGameSettings settings = kDefaultSettings;
  
  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state, nullptr);
  
  // Advance to potentially get some discards
  state = api::AdvanceGameState(state);
  EXPECT_NE(state, nullptr);
  
  api::CObservedGameState observed = api::ObserveGameState(state);
  
  // Test that FreeObservedGameState doesn't crash and properly nulls pointers
  api::FreeObservedGameState(&observed);
  
  // After freeing, all pointers should be null
  for (auto & player : observed.players) {
    EXPECT_EQ(player, nullptr);
  }
  
  // Test that calling FreeObservedGameState again doesn't crash
  api::FreeObservedGameState(&observed);
  
  // Test with nullptr
  api::FreeObservedGameState(nullptr);
  
  api::FreeGameState(state);
}

TEST(Api, ObserveGameStateStateFunctions) {
  const api::CGameSettings settings = kDefaultSettings;
  
  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state, nullptr);
  
  api::CObservedGameState observed = api::ObserveGameState(state);
  
  // Initial state should have GameStart as next state
  EXPECT_STREQ(observed.nextState, "GameStart");
  
  // Advance and check state transitions
  api::FreeObservedGameState(&observed);
  state = api::AdvanceGameState(state);
  observed = api::ObserveGameState(state);
  
  // After GameStart, next should be RoundStart
  EXPECT_STREQ(observed.nextState, "RoundStart");
  EXPECT_STREQ(observed.currState, "GameStart");
  
  // Clean up
  api::FreeObservedGameState(&observed);
  api::FreeGameState(state);
}

}  // namespace
