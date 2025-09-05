#pragma once
#include <vector>

#include "piecetype.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"

namespace mahjong {
class Walls {
 public:
  static void New(GameState& state);
  static Piece TakePiece(GameState& state);
  static std::vector<Piece> TakeHand(GameState& state);
  static Piece TakeReplacementTile(GameState& state);
  [[nodiscard]] static std::vector<Piece> GetDoras(const GameState& state);
  [[nodiscard]] static std::vector<Piece> GetUraDoras(const GameState& state);
  [[nodiscard]] static int GetRemainingPieces(const GameState& state);
};
}  // namespace mahjong
