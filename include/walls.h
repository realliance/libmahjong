#pragma once
#include <iosfwd>
#include <random>
#include <vector>

#include "piecetype.h"

namespace Mahjong {
class Walls {
 public:
  explicit Walls();
  explicit Walls(std::mt19937& g);
  explicit Walls(std::vector<Piece> wall);
  std::vector<Piece> livingWalls;
  std::vector<Piece> deadWall;
  int doraCount = 1;
  int replacements = 4;
  auto TakePiece() -> Piece;
  auto TakeHand() -> std::vector<Piece>;
  auto TakeReplacementTile() -> Piece;
  [[nodiscard]] auto GetDoras() const -> std::vector<Piece>;
  [[nodiscard]] auto GetUraDoras() const -> std::vector<Piece>;
  [[nodiscard]] auto GetRemainingPieces() const -> int;
};
}  // namespace Mahjong

auto operator<<(std::ostream& os, const Mahjong::Walls& walls) -> std::ostream&;
