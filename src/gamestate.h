#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include "hand.h"
#include "piecetype.h"
#include "player.h"
#include "walls.h"

namespace mahjong {

struct GameState {
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
  struct GameState& (*prevState)(struct GameState&);
  struct GameState& (*currState)(struct GameState&);
  struct GameState& (*nextState)(struct GameState&);
  Walls walls;
  std::array<int, 4> scores = {};
  std::array<bool, 4> hasRonned = {};
  std::array<Hand, 4> hands = {};
  std::array<Player, 4> players = {};
  std::vector<Piece> overrideWall;
};

}  // namespace mahjong

std::ostream& operator<<(std::ostream& os, const mahjong::GameState& state);
