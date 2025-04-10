#include "event.h"

#include <iostream>

#include "piecetype.h"

std::ostream& operator<<(std::ostream& os, const mahjong::Event& e) {
  os << "{type: " << mahjong::EventTypeToStr(e.type);
  os << " player: " << e.player;
  uint8_t p = e.piece;
  os << " piece: " << mahjong::Piece(p).toStr();
  os << " decision: " << e.decision << "}";
  return os;
}
