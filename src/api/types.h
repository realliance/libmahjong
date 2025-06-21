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

typedef struct CObservedGameState {
  int currentPlayer;
  int turnNum;
  int roundNum;
  int riichiSticks;
  int counters;
  int lastCall;
  int lastCaller;
  bool concealedKan;
  uint64_t seed;
  CPiece pendingPiece;
  int scores[4];
  int points[4];  // Player points
  bool hasRonned[4];
  CPiece hands[4][14];
  CPiece* players[4];  // Pointers to discard arrays for each player
  int playerDiscardCounts[4];  // Number of discards for each player
};
}
} // namespace api
