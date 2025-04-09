#include "meld.h"

#include <ostream>

auto operator<<(std::ostream& os, const mahjong::Meld& meld) -> std::ostream& {
  os << "{ type: " << MeldTypeToStr(meld.type);
  os << ", start: " << meld.start.toStr() << "}";
  return os;
}

namespace mahjong {
auto MeldTypeToStr(Meld::Type s) -> std::string {
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
