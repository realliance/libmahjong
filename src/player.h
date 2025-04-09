#pragma once
#include <iostream>

namespace mahjong {
class PlayerController;

struct Player {
  int points{};
  PlayerController* controller = nullptr;
};

};  // namespace mahjong

auto operator<<(std::ostream& os, const mahjong::Player& player)
    -> std::ostream&;
