#include <iostream>
#include <memory>

#include "statefunctions/statefunctions.h"

namespace mahjong {
struct GameState;

std::unique_ptr<GameState> Error(std::unique_ptr<GameState> state) {
  std::cerr << "ERROR STATE REACHED" << '\n';
  std::cerr << "GameState: " << '\n';
  std::cerr << state;
  throw "ERROR STATE REACHED";
}
}  // namespace mahjong
