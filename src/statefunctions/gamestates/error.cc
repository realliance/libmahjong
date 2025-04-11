#include <iostream>

#include "statefunctions/statefunctions.h"
#include "types/gamestate.h"
#include "types/typeprinter.h"

namespace mahjong {

GameState&& Error(GameState&& state) {
  std::cerr << "ERROR STATE REACHED" << '\n';
  std::cerr << "GameState: " << '\n';
  std::cerr << state;
  throw "ERROR STATE REACHED";
}
}  // namespace mahjong
