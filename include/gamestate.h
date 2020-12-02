#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include "hand.h"  // IWYU pragma: keep
#include "piecetype.h"
#include "player.h"  // IWYU pragma: keep
#include "walls.h"

namespace Mahjong {

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
  Piece pendingPiece = Piece::ERROR;
  auto (*prevState)(struct GameState&) -> struct GameState&;
  auto (*currState)(struct GameState&) -> struct GameState&;
  auto (*nextState)(struct GameState&) -> struct GameState&;
  Walls walls;
  std::array<int, 4> scores = {};
  std::array<bool, 4> hasRonned = {};
  std::array<Hand, 4> hands = {};
  std::array<Player, 4> players = {};
  std::vector<Piece> overrideWall;
};

}  // namespace Mahjong

auto operator<<(std::ostream& os, const Mahjong::GameState& state) -> std::ostream&;
