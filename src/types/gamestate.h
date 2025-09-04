#pragma once
#include <array>
#include <cstdint>
#include <random>
#include <ranges>

#include "controllers/playercontroller.h"
#include "types/meld.h"
#include "types/piecetype.h"
#include "types/settings.h"
#include "types/statefunction.h"

namespace mahjong {

struct Hand {
  bool open = false;
  bool riichi = false;
  int riichiPieceDiscard = -1;
  int riichiRound = -1;
  std::array<Piece, 14> live;
  int live_count = 13;
  std::array<Meld, 4> melds;
  int meld_count = 0;
  std::array<Piece, kMaxDiscardCount> discards;
  int discards_count = 0;
  [[nodiscard]] std::ranges::subrange<const Piece*> live_range() const {
    return std::ranges::subrange(live.begin(), live.begin() + live_count);
  }
  [[nodiscard]] std::ranges::subrange<const Meld*> melds_range() const {
    return std::ranges::subrange(melds.begin(), melds.begin() + meld_count);
  }
};

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
  int doraCount = 1;
  Piece pendingPiece = Piece(Piece::Type::kError);
  StateFunctionType prevState;
  StateFunctionType currState;
  StateFunctionType nextState;
  std::array<Piece, kLivingWallCount> livingWall;
  int livingWallIndex = 0;
  std::array<Piece, kDeadWallCount> deadWall;
  int deadWallIndex = 0;
  std::array<int, kNumPlayers> scores = {};
  std::array<bool, kNumPlayers> hasRonned = {};
  std::array<Hand, kNumPlayers> hands = {};
  std::array<std::unique_ptr<PlayerController>, kNumPlayers> players = {};
  std::array<int, kNumPlayers> points = {};
};

}  // namespace mahjong
