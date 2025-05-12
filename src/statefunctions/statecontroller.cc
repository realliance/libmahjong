#include "statecontroller.h"

#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <thread>
#include <utility>
#include <vector>

#include "controllers/controllermanager.h"
#include "statefunctions.h"
#include "types/gamestate.h"
#include "types/settings.h"

namespace mahjong {

namespace {
int thread_index = 1;
std::map<int, bool> should_halt;
}  // namespace

int StartGame(const GameSettings& settings, bool async) {
  if (async) {
    std::thread gameloop(&StateController, settings);
    gameloop.detach();
    return thread_index;
  }
  StateController(settings);
  return 0;
}

void ExitGame(int game) {
  if (should_halt.contains(game)) {
    // std::cerr << "Halting Game..." << std::endl;
    should_halt[game] = true;
  }
}

std::unique_ptr<GameState> InitGameState(const GameSettings& settings) {
  auto state = std::make_unique<GameState>();
  for (int i = 0; i < 4; i++) {
    state->players.at(i).controller = ControllerManager::Instance().NewController(settings.seatControllers.at(i));
  }
  if (settings.seed != 0U) {
    state->seed = settings.seed;
  } else {
    std::random_device rd;
    state->seed = rd();
  }
  state->nextState = GameStart;
  return state;
}

std::unique_ptr<GameState> AdvanceGameState(
    std::unique_ptr<GameState> state) {
  try {
    state->prevState = state->currState;
    state->currState = state->nextState;
    return state->nextState(std::move(state));
  } catch (const unsigned int e) {
    switch (e) {
      case 0xBAD22222:  // Asked for decision too many times.
        std::cerr << "Asked for decision too many times" << '\n';
        state->nextState = Error;
        break;
      default:
        throw(e);
    }
  }
  return state;
}

void StateController(const GameSettings& settings) {
  const int id = thread_index++;
  should_halt[id] = false;
  std::unique_ptr<GameState> state = InitGameState(settings);
  while (state->nextState != GameEnd && !should_halt[id]) {
    state = AdvanceGameState(std::move(state));
  }
  if (state->nextState == GameEnd) {
    state->nextState(std::move(state));
  }
}

}  // namespace mahjong
