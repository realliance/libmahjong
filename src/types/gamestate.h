#pragma once
#include <array>
#include <cstdint>
#include <random>
#include <ranges>

#include "controllers/playercontroller.h"
#include "types/hand.h"
#include "types/meld.h"
#include "types/piecetype.h"
#include "types/settings.h"
#include "types/statefunction.h"

namespace mahjong {

struct GameState {
  int currentPlayer = -1;
  int turnNum = -1;
  int roundNum = 0;
  int riichiSticks = 0;
  int counters = 0;
  int lastCall = -1;
  int lastCaller = -1;
  int livingWallIndex = 0;
  int deadWallIndex = 0;
  int doraCount = 1;
  bool concealedKan = false;
  Piece pendingPiece = Piece(Piece::Type::kError);

  std::array<Hand, kNumPlayers> players;
  std::array<Piece, kLivingWallCount> livingWall;
  std::array<Piece, kDeadWallCount> deadWall;

  uint64_t seed = 0;
  std::mt19937_64 g;
  std::array<std::unique_ptr<PlayerController>, kNumPlayers> controllers = {};

  StateFunctionType prevState;
  StateFunctionType currState;
  StateFunctionType nextState;
};

}  // namespace mahjong
