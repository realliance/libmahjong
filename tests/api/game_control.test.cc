#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "api/gamestate.h"
#include "api/types.h"
#include "types/gamestate.h"
#include "types/piecetype.h"

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
    EXPECT_EQ(state->players[i]->Name(), settings.seat_controllers[i]);
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

  api::CObservedGameState const observed = api::ObserveGameState(state);

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
  EXPECT_EQ(observed.pendingPiece,
            static_cast<api::CPiece>(state->pendingPiece.toUint8_t()));

  // Check state function name, the rest will be in unknown positions
  // until we advance the game state
  EXPECT_EQ(observed.nextState,
            api::CStateFunctionType::kGameStart);  // Initial state should be
                                                   // GameStart

  // Check arrays are properly sized and initialized
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(observed.scores[i], state->scores[i]);
    EXPECT_EQ(observed.points[i], state->points[i]);
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

  api::CObservedGameState const observed = api::ObserveGameState(state);

  // Verify the observation reflects the advanced state
  EXPECT_EQ(observed.currentPlayer, state->currentPlayer);
  EXPECT_EQ(observed.turnNum, state->turnNum);
  EXPECT_EQ(observed.roundNum, state->roundNum);
  EXPECT_EQ(observed.seed, state->seed);

  // After advancement, some fields should have changed from initial values
  // (exact values depend on game logic, but we can check they're copied
  // correctly)
  EXPECT_EQ(observed.riichiSticks, state->riichiSticks);
  EXPECT_EQ(observed.counters, state->counters);

  // Check state function names are valid
  EXPECT_NE(observed.prevState, api::CStateFunctionType::kError);
  EXPECT_NE(observed.currState, api::CStateFunctionType::kError);
  EXPECT_NE(observed.nextState, api::CStateFunctionType::kError);

  // Check points are copied
  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(observed.points[i], state->points[i]);
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

  api::CObservedGameState const observed = api::ObserveGameState(state);

  // Check that hands are copied (after round start, players should have hands)
  for (int player = 0; player < 4; player++) {
    const api::CHand& c_hand = observed.hands[player];
    const mahjong::Hand& cpp_hand = state->hands[player];

    // Check live pieces
    EXPECT_EQ(c_hand.livePieceCount, static_cast<int>(cpp_hand.live.size()));
    for (int piece = 0; piece < api::kMaxLiveHandSize; piece++) {
      if (piece < c_hand.livePieceCount) {
        EXPECT_EQ(c_hand.livePieces[piece],
                  static_cast<api::CPiece>(cpp_hand.live[piece].toUint8_t()));
      } else {
        // Should be filled with error pieces
        EXPECT_EQ(c_hand.livePieces[piece],
                  static_cast<api::CPiece>(mahjong::Piece::Type::kError));
      }
    }

    // Check melds
    EXPECT_EQ(c_hand.meldCount, static_cast<int>(cpp_hand.melds.size()));
    for (int meld = 0; meld < c_hand.meldCount; meld++) {
      EXPECT_EQ(static_cast<int>(c_hand.melds[meld].type),
                static_cast<int>(cpp_hand.melds[meld].type));
      EXPECT_EQ(
          c_hand.melds[meld].start,
          static_cast<api::CPiece>(cpp_hand.melds[meld].start.toUint8_t()));
    }

    // Check discards using CHand structure
    EXPECT_EQ(c_hand.discardCount, static_cast<int>(cpp_hand.discards.size()));
    for (int discard = 0; discard < c_hand.discardCount; discard++) {
      EXPECT_EQ(
          c_hand.discards[discard],
          static_cast<api::CPiece>(cpp_hand.discards[discard].toUint8_t()));
    }

    // Check hand properties
    EXPECT_EQ(c_hand.open, cpp_hand.open);
    EXPECT_EQ(c_hand.riichi, cpp_hand.riichi);
    EXPECT_EQ(c_hand.riichiPieceDiscard,
              static_cast<int>(cpp_hand.riichiPieceDiscard));
    EXPECT_EQ(c_hand.riichiRound, cpp_hand.riichiRound);
  }

  api::FreeGameState(state);
}

TEST(Api, ObserveGameStateStateFunctions) {
  const api::CGameSettings settings = kDefaultSettings;

  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state, nullptr);

  api::CObservedGameState observed = api::ObserveGameState(state);

  // Initial state should have GameStart as next state
  EXPECT_EQ(observed.nextState, api::CStateFunctionType::kGameStart);

  // Advance and check state transitions
  state = api::AdvanceGameState(state);
  observed = api::ObserveGameState(state);

  // After GameStart, next should be RoundStart
  EXPECT_EQ(observed.nextState, api::CStateFunctionType::kRoundStart);
  EXPECT_EQ(observed.currState, api::CStateFunctionType::kGameStart);

  // Clean up
  api::FreeGameState(state);
}

TEST(Api, GameControllerValidation) {
  const api::CGameSettings settings = kDefaultSettings;

  // Check valid controllers
  for (const auto& controller : settings.seat_controllers) {
    EXPECT_TRUE(api::IsValidGameController(controller));
  }

  // Check invalid controller
  EXPECT_FALSE(api::IsValidGameController("InvalidController"));

  // If an invalid controller is used, InitGameState should return nullptr
  api::CGameSettings invalid_settings = settings;
  invalid_settings.seat_controllers[0] = "InvalidController";
  mahjong::GameState* state = api::InitGameState(&invalid_settings);
  EXPECT_EQ(state, nullptr);
}

}  // namespace
