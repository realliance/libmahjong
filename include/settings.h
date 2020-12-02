#pragma once
#include <string>
#include <vector>

#include "piecetype.h"

namespace Mahjong {

struct GameSettings {
  std::vector<std::string> seatControllers{};
  std::vector<Piece> overrideWall{};
  uint64_t seed = 0;
};

}  // namespace Mahjong
