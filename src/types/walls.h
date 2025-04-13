#pragma once
#include <random>
#include <vector>

#include "piecetype.h"

namespace mahjong {
class Walls {
 public:
  explicit Walls();
  explicit Walls(std::mt19937& g);
  explicit Walls(std::vector<Piece> wall);
  std::vector<Piece> livingWalls;
  std::vector<Piece> deadWall;
  int doraCount = 1;
  int replacements = 4;
  Piece TakePiece();
  std::vector<Piece> TakeHand();
  Piece TakeReplacementTile();
  [[nodiscard]] std::vector<Piece> GetDoras() const;
  [[nodiscard]] std::vector<Piece> GetUraDoras() const;
  [[nodiscard]] int GetRemainingPieces() const;
};
}  // namespace mahjong
