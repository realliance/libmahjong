#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <memory>

#include "api/gamestate.h"
#include "api/types.h"
#include "types/gamestate.h"
#include "types/hand.h"
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
  EXPECT_EQ(state->controllers.size(), settings.num_controllers);
  for (uint64_t i = 0; i < state->controllers.size(); ++i) {
    EXPECT_EQ(state->controllers[i]->Name(), settings.seat_controllers[i]);
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
    EXPECT_EQ(observed.scores[i], state->players[i].score);
    EXPECT_EQ(observed.points[i], state->players[i].points);
    EXPECT_EQ(observed.hasRonned[i], state->players[i].hasRonned);
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
    EXPECT_EQ(observed.points[i], state->players[i].points);
  }

  api::FreeGameState(state);
}

TEST(Api, ObserveGameStatePlayersAndDiscards) {
  const api::CGameSettings settings = kDefaultSettings;

  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state, nullptr);

  // Advance to get some hands dealt
  state = api::AdvanceGameState(state);
  EXPECT_NE(state, nullptr);

  api::CObservedGameState const observed = api::ObserveGameState(state);

  // Check that hands are copied (after round start, players should have hands)
  for (int player = 0; player < 4; player++) {
    const api::CPlayer& c_player = observed.hands[player];
    const mahjong::Player& cpp_player = state->players[player];

    // Check live pieces
    EXPECT_EQ(c_player.livePieceCount, static_cast<int>(cpp_player.live_count));
    for (int piece = 0; piece < api::kMaxLivePlayerSize; piece++) {
      if (piece < c_player.livePieceCount) {
        EXPECT_EQ(c_player.livePieces[piece],
                  static_cast<api::CPiece>(cpp_player.live[piece].toUint8_t()));
      } else {
        // Should be filled with error pieces
        EXPECT_EQ(c_player.livePieces[piece],
                  static_cast<api::CPiece>(mahjong::Piece::Type::kError));
      }
    }

    // Check melds
    EXPECT_EQ(c_player.meldCount, cpp_player.meld_count);
    for (int meld = 0; meld < c_player.meldCount; meld++) {
      EXPECT_EQ(static_cast<int>(c_player.melds[meld].type),
                static_cast<int>(cpp_player.melds[meld].type));
      EXPECT_EQ(
          c_player.melds[meld].start,
          static_cast<api::CPiece>(cpp_player.melds[meld].start.toUint8_t()));
    }

    // Check discards using CPlayer structure
    EXPECT_EQ(c_player.discardCount, static_cast<int>(cpp_player.discards_count));
    for (int discard = 0; discard < c_player.discardCount; discard++) {
      EXPECT_EQ(
          c_player.discards[discard],
          static_cast<api::CPiece>(cpp_player.discards[discard].toUint8_t()));
    }

    // Check hand properties
    EXPECT_EQ(c_player.open, cpp_player.open);
    EXPECT_EQ(c_player.riichi, cpp_player.riichi);
    EXPECT_EQ(c_player.riichiPieceDiscard,
              static_cast<int>(cpp_player.riichiPieceDiscard));
    EXPECT_EQ(c_player.riichiRound, cpp_player.riichiRound);
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
