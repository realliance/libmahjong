#include "gamestate.h"
#include <memory>
#include <vector>
#include "../statefunctions/statecontroller.h"
#include "types.h"

namespace api {

namespace {

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
}  // namespace

extern "C" {

int StartGame(const CGameSettings* settings, bool async) {
  const mahjong::GameSettings cpp_settings = convertGameSettings(settings);
  return mahjong::StartGame(cpp_settings, async);
}

void ExitGame(int game) {
  mahjong::ExitGame(game);
}

mahjong::GameState* InitGameState(const CGameSettings* settings) {
  const mahjong::GameSettings cpp_settings = convertGameSettings(settings);
  std::unique_ptr<mahjong::GameState> state =
      mahjong::InitGameState(cpp_settings);
  return state.release();
}

mahjong::GameState* AdvanceGameState(mahjong::GameState* state) {
  std::unique_ptr<mahjong::GameState> new_state =
      mahjong::AdvanceGameState(std::unique_ptr<mahjong::GameState>(state));
  return new_state.release();
}

void FreeGameState(mahjong::GameState* state) {
  delete state;
}
}
}  // namespace api
