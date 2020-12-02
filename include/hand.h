#pragma once
#include <algorithm>
#include <cstddef>
#include <iosfwd>
#include <utility>
#include <vector>

#include "meld.h"
#include "piecetype.h"

namespace Mahjong {

class Hand {
 public:
  Hand() = default;
  explicit Hand(std::vector<Piece> live) : live(std::move(std::move(live))) {}
  auto inline sort() -> void {
    std::sort(live.begin(), live.end());
  }
  std::vector<Piece> live;
  std::vector<Meld> melds;
  std::vector<Piece> discards;
  bool open = false;
  bool riichi = false;
  size_t riichiPieceDiscard = -1;
  int riichiRound = -1;
};

}  // namespace Mahjong

auto operator<<(std::ostream& os, const Mahjong::Hand& hand) -> std::ostream&;
