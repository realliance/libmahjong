#pragma once

#include <array>

#include "types/meld.h"
#include "types/piecetype.h"
#include "types/settings.h"
namespace mahjong {

struct Hand {
  int id;
  std::array<Piece, kMaxDiscardCount> discards;
  int discards_count = 0;
  bool riichi = false;
  int riichiPieceDiscard;
  int riichiRound;
  int score;
  bool hasRonned;
  int points;
  bool open;
  std::array<Piece, 14> live;
  int live_count = 13;
  std::array<Meld, 4> melds;
  int meld_count = 0;
  [[nodiscard]] std::ranges::subrange<const Piece*> live_range() const {
    return std::ranges::subrange(live.begin(), live.begin() + live_count);
  }
  [[nodiscard]] std::ranges::subrange<const Meld*> melds_range() const {
    return std::ranges::subrange(melds.begin(), melds.begin() + meld_count);
  }
};
}  // namespace mahjong
