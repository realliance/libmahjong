#include "event.h"

#include <iostream>

#include "piecetype.h"

using Mahjong::Event, Mahjong::EventTypeToStr, Mahjong::Piece;

auto operator<<(std::ostream& os, const Event& e) -> std::ostream& {
  os << "{type: " << EventTypeToStr(e.type);
  os << " player: " << e.player;
  uint8_t p = e.piece;
  os << " piece: " << Piece(p).toStr();
  os << " decision: " << e.decision << "}";
  return os;
}
