#pragma once
#include <iostream>

namespace Mahjong {
class PlayerController;

struct Player {
  int points{};
  PlayerController* controller = nullptr;
};

};  // namespace Mahjong

auto operator<<(std::ostream& os, const Mahjong::Player& player) -> std::ostream&;
