#pragma once
#include <string>

#include "types/piecetype.h"
#include "types/sets.h"

namespace mahjong {

struct Meld {
  SetType type;
  Piece start;

  bool operator==(Meld other) const {
    if (type != other.type) {
      return false;
    }
    return start == other.start;
  }

  [[nodiscard]] std::string typeToStr() const {
    switch (type) {
      case SetType::kChi:
        return "Chi";
      case SetType::kKan:
        return "Kan";
      case SetType::kPon:
        return "Pon";
      case SetType::kConcealedKan:
        return "ConcealedKan";
      default:
        return "InvalidMeldType";
    }
  }
};

}  // namespace mahjong
