#pragma once

#include <memory>
#include "stdint.h"
#include "types/gamestate.h"

namespace api {

extern "C" {

// Mahjong game constants
static const int kMaxLiveHandSize = 14;      // Maximum 14 pieces (13 + 1 drawn)
static const int kMaxMeldsPerHand = 4;       // Maximum 4 melds possible in mahjong
static const int kMaxDiscardsPerPlayer = 21; // Maximum 21 discards per player ((136 - 13*4 hands) / 4)

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

typedef enum CMeldType {
  kChi = 0,
  kPon = 1,
  kKan = 2,
  kConcealedKan = 3,
  kNone = 4 // Default value
} CMeldType;

typedef struct CMeld {
  CMeldType type;
  CPiece start;
};

typedef struct CHand {
  CPiece live[kMaxLiveHandSize];           // Maximum live pieces
  int liveCount;                           // Number of live pieces
  CMeld melds[kMaxMeldsPerHand];           // Maximum melds possible
  int meldCount;                           // Number of melds
  CPiece discards[kMaxDiscardsPerPlayer];  // Maximum discards per player
  int discardCount;                        // Number of discards
  bool open;                               // Whether hand is open
  bool riichi;                             // Whether player declared riichi
  int riichiPieceDiscard;                  // Index of riichi discard piece (-1 if none)
  int riichiRound;                         // Round when riichi was declared (-1 if none)
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
  CHand hands[4];        // Player hands with live pieces, melds, and discards
  const char* prevState;  // Previous state function name
  const char* currState;  // Current state function name
  const char* nextState;  // Next state function name
};
}
} // namespace api
