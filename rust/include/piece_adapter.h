#pragma once
#include "types/piecetype.h"

namespace mahjong {

// Piece::Type is hard to map into rust, formPieceFromU8 allows us to supply
// a uint8_t for the suit instead
inline Piece formPieceFromU8(uint8_t suit, uint8_t number,
                             bool is_red_five = false) {
  return Piece::formPiece(static_cast<Piece::Type>(suit), number, is_red_five);
}

}  // namespace mahjong
