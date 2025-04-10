#pragma once
#include <iosfwd>
#include <ostream>
#include <string>

#include "piecetype.h"

namespace mahjong {

struct Meld {
  enum Type {  // order here is important. add any new types carefully
    kChi,
    kPon,
    kKan,
    kConcealedKan
  };
  Type type;
  Piece start;

  bool operator==(Meld other) const {
    if (type != other.type) {
      return false;
    }
    return start == other.start;
  }

  std::string typeToStr() const {
    switch (type) {
      case Meld::kChi:
        return "Chi";
      case Meld::kKan:
        return "Kan";
      case Meld::kPon:
        return "Pon";
      case Meld::kConcealedKan:
        return "ConcealedKan";
      default:
        return "InvalidMeldType";
    }
  }
};

}  // namespace mahjong
