#include "gamestate.h"
#include <utility>
#include <vector>
#include "../statefunctions/statecontroller.h"
#include "types.h"

mahjong::GameSettings convertGameSettings(const CGameSettings* settings) {
  mahjong::GameSettings cpp_settings;

  cpp_settings.seed = settings->seed;

  // Char arrays to vector
  for (int i = 0; i < settings->num_controllers; i++) {
    if (settings->seat_controllers[i]) {
      cpp_settings.seatControllers.emplace_back(settings->seat_controllers[i]);
    }
  }

  return cpp_settings;
}

extern "C" {

int StartGame(const CGameSettings* settings, int async) {
  const mahjong::GameSettings cpp_settings = convertGameSettings(settings);
  return mahjong::StartGame(cpp_settings, async != 0);
}

void ExitGame(int game) {
  mahjong::ExitGame(game);
}

CGameState* InitGameState(const CGameSettings* settings) {
  const mahjong::GameSettings cpp_settings = convertGameSettings(settings);
  auto state = mahjong::InitGameState(cpp_settings);
  return new CGameState{std::move(state)};
}

CGameState* AdvanceGameState(CGameState* state) {
  auto new_state = mahjong::AdvanceGameState(std::move(state->wrapped_state));
  delete state;
  return new CGameState{std::move(new_state)};
}

void DestroyGameState(CGameState* state) {
  delete state;
}
}