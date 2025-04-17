#pragma once
#include <string>
#include <vector>

#include "piecetype.h"

namespace mahjong {

constexpr int kNumPlayers = 4;  // Eventually will make this configurable (3/4).
constexpr int kStartingPoints = 25000;

struct GameSettings {
  std::vector<std::string> seatControllers;
  uint64_t seed = 0;
};

}  // namespace mahjong
