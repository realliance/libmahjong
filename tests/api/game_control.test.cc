#include <gtest/gtest.h>
#include "api/gamestate.h"
#include "api/types.h"

namespace mahjong {

TEST(Api, SettingsConversion) {
  const CGameSettings settings{
      .seed = 12345,
      .seat_controllers = {"AngryDiscardoBot", "AngryDiscardoBot",
                          "AngryDiscardoBot", "AngryDiscardoBot"},
      .num_controllers = 4,
  };

  auto converted_settings = convertGameSettings(&settings);

  EXPECT_EQ(converted_settings.seed, settings.seed);
  EXPECT_EQ(converted_settings.seatControllers.size(), settings.num_controllers);
  for (int i = 0; i < settings.num_controllers; ++i) {
    EXPECT_EQ(converted_settings.seatControllers[i].c_str(),
              settings.seat_controllers[i]);
  }
}

TEST(Api, InitGameState) {
  const CGameSettings settings{
      .seed = 12345,
      .seat_controllers = {"AngryDiscardoBot", "AngryDiscardoBot",
                          "AngryDiscardoBot", "AngryDiscardoBot"},
      .num_controllers = 4,
  };

  CGameState* state = InitGameState(&settings);
  EXPECT_NE(state->wrapped_state, nullptr);
}

TEST(Api, AdvanceGameState) {
  const CGameSettings settings{
      .seed = 12345,
      .seat_controllers = {"AngryDiscardoBot", "AngryDiscardoBot",
                          "AngryDiscardoBot", "AngryDiscardoBot"},
      .num_controllers = 4,
  };

  CGameState* state = InitGameState(&settings);
  EXPECT_NE(state->wrapped_state, nullptr);

  CGameState* new_state = AdvanceGameState(state);
  EXPECT_NE(new_state->wrapped_state, nullptr);
}

}  // namespace mahjong