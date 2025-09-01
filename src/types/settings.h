#pragma once
#include <string>
#include <vector>

#include "types/piecetype.h"

namespace mahjong {

// TODO(#24): Support 3 player mahjong
constexpr int kNumPlayers = 4;
// TODO(#25): Support configuring parts of scoring
constexpr int kStartingPoints = 25000;

struct GameSettings {
  std::vector<std::string> seatControllers;
  uint64_t seed = 0;
};

}  // namespace mahjong
