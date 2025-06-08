#include <gtest/gtest.h>
#include <cstdint>
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

}  // namespace
