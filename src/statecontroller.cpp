#include <array>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <thread>
#include <vector>

#include "controllermanager.h"
#include "gamestate.h"
#include "player.h"
#include "settings.h"
#include "statefunctions.h"

static int threadIndex = 1;
static std::map<int, bool> shouldHalt;

auto Mahjong::StartGame(const GameSettings& settings, bool async) -> int {
  if (async) {
    std::thread gameloop(&Mahjong::StateController, settings);
    gameloop.detach();
    return threadIndex;
  }
  StateController(settings);
  return 0;
}

auto Mahjong::ExitGame(int game) -> void {
  if (shouldHalt.contains(game)) {
    // std::cerr << "Halting Game..." << std::endl;
    shouldHalt[game] = true;
  }
}

auto Mahjong::StateController(GameSettings settings) -> void {
  GameState state;
  int id = threadIndex++;
  shouldHalt[id] = false;

  for (int i = 0; i < 4; i++) {
    state.players.at(i).controller = GetController(settings.seatControllers.at(i))();
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
  while (state.nextState != GameEnd && !shouldHalt[id]) {
    try {
      state.prevState = state.currState;
      state.currState = state.nextState;
      state = state.nextState(state);
    } catch (const unsigned int e) {
      switch (e) {
        case 0xFACEFEED:  //Halted during controller decision
          shouldHalt.erase(id);
          return;
        case 0xBAD22222:  //Asked for decision too many times.
          std::cerr << "Asked for decision too many times" << std::endl;
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
