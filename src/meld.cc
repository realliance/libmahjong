#include "meld.h"

#include <ostream>

std::ostream& operator<<(std::ostream& os, const mahjong::Meld& meld) {
  os << "{ type: " << MeldTypeToStr(meld.type);
  os << ", start: " << meld.start.toStr() << "}";
  return os;
}

namespace mahjong {
std::string MeldTypeToStr(Meld::Type s) {
  switch (s) {
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
}  // namespace mahjong
