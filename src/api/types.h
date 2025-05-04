#pragma once

#include <memory>
#include "../types/gamestate.h"

extern "C" {

typedef int CPiece;
typedef int CWind;

typedef struct CEvent {
  int type;
  int player;
  CPiece piece;
  bool decision;
} CEvent;

typedef struct CGameSettings {
  unsigned int seed;
  const char* seat_controllers[4];
  int num_controllers = 0;
} GameSettings;

typedef struct CGameState {
  std::unique_ptr<mahjong::GameState> wrapped_state;
} GameState;
}
