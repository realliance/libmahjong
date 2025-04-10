#include <iostream>

#include "gamestate.h"
#include "statefunctions.h"
#include "typeprinter.h"

namespace mahjong {
GameState& Error(GameState& state) {
  std::cerr << "ERROR STATE REACHED" << '\n';
  std::cerr << "GameState: " << '\n';
  std::cerr << state;
  throw "ERROR STATE REACHED";
}
}  // namespace mahjong
