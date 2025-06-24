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
    const api::CHand& cHand = observed.hands[player];
    const mahjong::Hand& cppHand = state->hands[player];
    
    // Check live pieces
    EXPECT_EQ(cHand.liveCount, static_cast<int>(cppHand.live.size()));
    for (int piece = 0; piece < api::kMaxLiveHandSize; piece++) {
      if (piece < cHand.liveCount) {
        EXPECT_EQ(cHand.live[piece], 
                  static_cast<api::CPiece>(cppHand.live[piece].toUint8_t()));
      } else {
        // Should be filled with error pieces
        EXPECT_EQ(cHand.live[piece], 
                  static_cast<api::CPiece>(mahjong::Piece::Type::kError));
      }
    }
    
    // Check melds
    EXPECT_EQ(cHand.meldCount, static_cast<int>(cppHand.melds.size()));
    for (int meld = 0; meld < cHand.meldCount; meld++) {
      EXPECT_EQ(static_cast<int>(cHand.melds[meld].type), static_cast<int>(cppHand.melds[meld].type));
      EXPECT_EQ(cHand.melds[meld].start, static_cast<api::CPiece>(cppHand.melds[meld].start.toUint8_t()));
    }
    
    // Check discards using CHand structure
    EXPECT_EQ(cHand.discardCount, static_cast<int>(cppHand.discards.size()));
    for (int discard = 0; discard < cHand.discardCount; discard++) {
      EXPECT_EQ(cHand.discards[discard],
                static_cast<api::CPiece>(cppHand.discards[discard].toUint8_t()));
    }
    
    // Check hand properties
    EXPECT_EQ(cHand.open, cppHand.open);
    EXPECT_EQ(cHand.riichi, cppHand.riichi);
    EXPECT_EQ(cHand.riichiPieceDiscard, static_cast<int>(cppHand.riichiPieceDiscard));
    EXPECT_EQ(cHand.riichiRound, cppHand.riichiRound);
  }
  
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
  state = api::AdvanceGameState(state);
  observed = api::ObserveGameState(state);
  
  // After GameStart, next should be RoundStart
  EXPECT_STREQ(observed.nextState, "RoundStart");
  EXPECT_STREQ(observed.currState, "GameStart");
  
  // Clean up
  api::FreeGameState(state);
}

}  // namespace
