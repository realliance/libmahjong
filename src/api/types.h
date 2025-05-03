#pragma once

#include "../types/gamestate.h"
#include <memory>

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
    char** seatControllers; // Array of controller names
    int numControllers;  // Number of controllers
    char* overrideWall;     // Optional override wall
  } GameSettings;

  // Define CGameState struct instead of just declaring it
  typedef struct CGameState {
    std::unique_ptr<mahjong::GameState> state;
  } CGameState;
  
  typedef struct CPlayerController PlayerController;
}
