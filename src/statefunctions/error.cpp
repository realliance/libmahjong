#include <iostream>

#include "gamestate.h"
#include "statefunctions.h"

auto Mahjong::Error(GameState& state) -> GameState& {
  std::cerr << "ERROR STATE REACHED" << std::endl;
  std::cerr << "GameState: " << std::endl;
  std::cerr << state;
  throw "ERROR STATE REACHED";
}
