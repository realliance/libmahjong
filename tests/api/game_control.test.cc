#include <gtest/gtest.h>
#include "api/gamestate.h"
#include "api/types.h"
#include "types/gamestate.h"

namespace mahjong {

TEST(Api, SettingsConversion) {
  const api::CGameSettings settings{
      .seed = 12345,
      .seat_controllers = {"AngryDiscardoBot", "AngryDiscardoBot",
                          "AngryDiscardoBot", "AngryDiscardoBot"},
      .num_controllers = 4,
  };

  auto converted_settings = api::convertGameSettings(&settings);

  EXPECT_EQ(converted_settings.seed, settings.seed);
  EXPECT_EQ(converted_settings.seatControllers.size(), settings.num_controllers);
  for (int i = 0; i < settings.num_controllers; ++i) {
    EXPECT_EQ(converted_settings.seatControllers[i],
              settings.seat_controllers[i]);
  }
}

TEST(Api, InitGameState) {
  const api::CGameSettings settings{
      .seed = 12345,
      .seat_controllers = {"AngryDiscardoBot", "AngryDiscardoBot",
                          "AngryDiscardoBot", "AngryDiscardoBot"},
      .num_controllers = 4,
  };

  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state, nullptr);
}

TEST(Api, AdvanceGameState) {
  const api::CGameSettings settings{
      .seed = 12345,
      .seat_controllers = {"AngryDiscardoBot", "AngryDiscardoBot",
                          "AngryDiscardoBot", "AngryDiscardoBot"},
      .num_controllers = 4,
  };

  mahjong::GameState* state = api::InitGameState(&settings);
  EXPECT_NE(state->seed, 0);

  mahjong::GameState* new_state = api::AdvanceGameState(state);
  EXPECT_NE(new_state, nullptr);
}

}  // namespace mahjong