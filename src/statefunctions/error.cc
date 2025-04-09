#include <iostream>

#include "gamestate.h"
#include "statefunctions.h"

auto mahjong::Error(GameState& state) -> GameState& {
  std::cerr << "ERROR STATE REACHED" << '\n';
  std::cerr << "GameState: " << '\n';
  std::cerr << state;
  throw "ERROR STATE REACHED";
}
