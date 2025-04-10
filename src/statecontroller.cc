#include <array>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "controllermanager.h"
#include "gamestate.h"
#include "player.h"
#include "settings.h"
#include "statefunctions.h"

namespace {
int thread_index = 1;
std::map<int, bool> should_halt;
}  // namespace

int mahjong::StartGame(const GameSettings& settings, bool async) {
  if (async) {
    std::thread gameloop(&mahjong::StateController, settings);
    gameloop.detach();
    return thread_index;
  }
  StateController(settings);
  return 0;
}

void mahjong::ExitGame(int game) {
  if (should_halt.contains(game)) {
    // std::cerr << "Halting Game..." << std::endl;
    should_halt[game] = true;
  }
}

void mahjong::StateController(GameSettings settings) {
  GameState state;
  int id = thread_index++;
  should_halt[id] = false;

  for (int i = 0; i < 4; i++) {
    state.players.at(i).controller =
        GetController(settings.seatControllers.at(i))();
  }
  if (settings.seed != 0U) {
    state.seed = settings.seed;
  } else {
    std::random_device rd;
    state.seed = rd();
  }
  if (!settings.overrideWall.empty()) {
    std::swap(state.overrideWall, settings.overrideWall);
    state.seed = 0xBEEFBABE;
  }
  state.prevState = nullptr;
  state.currState = GameStart;
  state.nextState = GameStart;
  while (state.nextState != GameEnd && !should_halt[id]) {
    try {
      state.prevState = state.currState;
      state.currState = state.nextState;
      state = state.nextState(state);
    } catch (const unsigned int e) {
      switch (e) {
        case 0xFACEFEED:  // Halted during controller decision
          should_halt.erase(id);
          return;
        case 0xBAD22222:  // Asked for decision too many times.
          std::cerr << "Asked for decision too many times" << '\n';
          state.nextState = Error;
          break;
        default:
          throw(e);
      }
    }
  }
  if (state.nextState == GameEnd) {
    state.nextState(state);
  }
}
