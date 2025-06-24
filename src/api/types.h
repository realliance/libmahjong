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
  kMeldChi = 0,
  kMeldPon = 1,
  kMeldKan = 2,
  kMeldConcealedKan = 3,
  kMeldNone = 4 // Default value
} CMeldType;

// C API version of StateFunctionType enum
typedef enum CStateFunctionType {
  kError = 0,
  kGameStart = 1,
  kRoundStart = 2,
  kDraw = 3,
  kPlayerHand = 4,
  kPon = 5,
  kChi = 6,
  kKan = 7,
  kConcealedKan = 8,
  kConvertedKan = 9,
  kKanDiscard = 10,
  kReplacement = 11,
  kRiichi = 12,
  kDiscard = 13,
  kExhaust = 14,
  kRon = 15,
  kTsumo = 16,
  kRoundEnd = 17,
  kGameEnd = 18
} CStateFunctionType;

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
  CStateFunctionType prevState;  // Previous state function
  CStateFunctionType currState;  // Current state function
  CStateFunctionType nextState;  // Next state function
};
}
}  // namespace api
