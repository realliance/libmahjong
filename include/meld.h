#pragma once
#include <iosfwd>
#include <string>

#include "piecetype.h"

namespace Mahjong {

struct Meld {
  enum Type {  //order here is important. add any new types carefully
    Chi,
    Pon,
    Kan,
    ConcealedKan
  };
  Type type;
  Piece start;
  auto inline operator==(Meld other) const -> bool {
    if (type != other.type) {
      return false;
    }
    return start == other.start;
  }
};

inline auto MeldTypeToStr(Meld::Type s) -> std::string {
  switch (s) {
    case Meld::Chi:
      return "Chi";
    case Meld::Kan:
      return "Kan";
    case Meld::Pon:
      return "Pon";
    case Meld::ConcealedKan:
      return "ConcealedKan";
    default:
      return "InvalidMeldType";
  }
}

}  // namespace Mahjong

auto operator<<(std::ostream& os, const Mahjong::Meld& meld) -> std::ostream&;
