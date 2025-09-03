#pragma once
#include <array>
#include <cstdint>
#include <random>

#include "types/hand.h"
#include "types/piecetype.h"
#include "types/player.h"
#include "types/settings.h"
#include "types/statefunction.h"
#include "types/walls.h"

namespace mahjong {

struct GameState {
  int currentPlayer = -1;
  int turnNum = -1;
  int roundNum = 0;
  int riichiSticks = 0;
  int counters = 0;
  int lastCall = -1;
  int lastCaller = -1;
  bool concealedKan = false;
  uint64_t seed = 0;
  std::mt19937_64 g;
  Piece pendingPiece = Piece(Piece::Type::kError);
  StateFunctionType prevState;
  StateFunctionType currState;
  StateFunctionType nextState;
  Walls walls;
  std::array<int, kNumPlayers> scores = {};
  std::array<bool, kNumPlayers> hasRonned = {};
  std::array<Hand, kNumPlayers> hands = {};
  std::array<Player, kNumPlayers> players = {};
};

}  // namespace mahjong
