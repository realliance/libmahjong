#include "event.h"

#include <iostream>

#include "piecetype.h"

auto operator<<(std::ostream& os, const mahjong::Event& e) -> std::ostream& {
  os << "{type: " << mahjong::EventTypeToStr(e.type);
  os << " player: " << e.player;
  uint8_t p = e.piece;
  os << " piece: " << mahjong::Piece(p).toStr();
  os << " decision: " << e.decision << "}";
  return os;
}
