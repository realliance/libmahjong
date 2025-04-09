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
  auto operator==(Meld other) const -> bool {
    if (type != other.type) {
      return false;
    }
    return start == other.start;
  }
};

auto MeldTypeToStr(Meld::Type s) -> std::string;

}  // namespace mahjong

auto operator<<(std::ostream& os, const mahjong::Meld& meld) -> std::ostream&;
