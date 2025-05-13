#pragma once

#include <memory>
#include "stdint.h"
#include "types/gamestate.h"

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
