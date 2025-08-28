#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace mahjong {

// TODO(#24): Support 3 player mahjong
constexpr int kNumPlayers = 4;
// TODO(#25): Support configuring parts of scoring
constexpr int kStartingPoints = 25000;

constexpr int kTileCount = 136;
constexpr int kReplacementCount = kNumPlayers == 4 ? 4 : 8;
constexpr int kDoraCount = 5;
constexpr int kDeadWallCount = (2 * kDoraCount) + kReplacementCount;
constexpr int kLivingWallCount = kTileCount - kDeadWallCount;
constexpr int kMaxDiscardCount = 27;

struct GameSettings {
  std::vector<std::string> seatControllers;
  uint64_t seed = 0;
};

}  // namespace mahjong
