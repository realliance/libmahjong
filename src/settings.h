#pragma once
#include <string>
#include <vector>

#include "piecetype.h"

namespace mahjong {

struct GameSettings {
  std::vector<std::string> seatControllers;
  std::vector<Piece> overrideWall;
  uint64_t seed = 0;
};

}  // namespace mahjong
