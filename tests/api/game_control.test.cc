#include <gtest/gtest.h>
#include "api/gamestate.h"
#include "api/types.h"

namespace mahjong {

TEST(Api, SettingsConversion) {
  auto *controller = const_cast<char*>("AngryDiscardoBot");

  char* seat_controllers[] = {controller, controller, controller, controller};

  auto settings = CGameSettings {
    .seed = 12345,
    .seatControllers = seat_controllers,
    .numControllers = 4,
  };
  auto converted_settings = convertGameSettings(&settings);

  EXPECT_EQ(converted_settings.seed, settings.seed);
  EXPECT_EQ(converted_settings.seatControllers.size(), settings.numControllers);
  for (int i = 0; i < settings.numControllers; ++i) {
    EXPECT_EQ(converted_settings.seatControllers[i], seat_controllers[i]);
  }
}

TEST(Api, InitGameState) {
  auto *controller = const_cast<char*>("AngryDiscardoBot");

  char* seat_controllers[] = {controller, controller, controller, controller};

  auto settings = CGameSettings {
    .seed = 12345,
    .seatControllers = seat_controllers,
    .numControllers = 4,
  };

  CGameState* state = InitGameState(&settings);
  EXPECT_NE(state->wrapped_state, nullptr);
}

TEST(Api, AdvanceGameState) {
  auto *controller = const_cast<char*>("AngryDiscardoBot");

  char* seat_controllers[] = {controller, controller, controller, controller};

  auto settings = CGameSettings {
    .seed = 12345,
    .seatControllers = seat_controllers,
    .numControllers = 4,
  };

  CGameState* state = InitGameState(&settings);
  EXPECT_NE(state->wrapped_state, nullptr);

  CGameState* new_state = AdvanceGameState(state);
  EXPECT_NE(new_state->wrapped_state, nullptr);
}

} // namespace mahjong