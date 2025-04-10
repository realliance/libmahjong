#pragma once
#include <iostream>

#include "controllers/playercontroller.h"

namespace mahjong {

struct Player {
  int points{};
  PlayerController* controller = nullptr;
};

};  // namespace mahjong
