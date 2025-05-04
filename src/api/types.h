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
  char** seatControllers;
  int numControllers;
} GameSettings;

typedef struct CGameState {
  std::unique_ptr<mahjong::GameState> wrapped_state;
} GameState;
}
