#pragma once

#include <memory>
#include "types/gamestate.h"
#include "stdint.h"

namespace api {

extern "C" {

typedef int CPiece;
typedef int CWind;

typedef struct CEvent {
  int type;
  int player;
  CPiece piece;
  bool decision;
};

typedef struct CGameSettings {
  uint64_t seed;
  const char* seat_controllers[4];
  int num_controllers = 0;
};
}
} // namespace api
