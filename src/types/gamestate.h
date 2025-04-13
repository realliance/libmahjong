#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

#include "hand.h"
#include "piecetype.h"
#include "player.h"
#include "settings.h"
#include "walls.h"

namespace mahjong {

struct GameState {
  using StateFunction = GameState && (*)(GameState&&);
  bool halt = false;
  int currentPlayer = -1;
  int turnNum = -1;
  int roundNum = 0;
  int riichiSticks = 0;
  int counters = 0;
  int lastCall = -1;
  int lastCaller = -1;
  bool concealedKan = false;
  uint64_t seed = 0;
  std::mt19937 g;
  Piece pendingPiece = Piece(Piece::Type::kError);
  StateFunction prevState;
  StateFunction currState;
  StateFunction nextState;
  Walls walls{};
  std::array<int, kNumPlayers> scores = {};
  std::array<bool, kNumPlayers> hasRonned = {};
  std::array<Hand, kNumPlayers> hands = {};
  std::array<Player, kNumPlayers> players = {};
  std::vector<Piece> overrideWall;
};

}  // namespace mahjong
