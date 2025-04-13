#pragma once
#include <iostream>
#include <memory>

#include "controllers/playercontroller.h"

namespace mahjong {

struct Player {
  int points;
  std::unique_ptr<PlayerController> controller;
};

};  // namespace mahjong
