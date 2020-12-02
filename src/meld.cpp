#include "meld.h"

#include <ostream>

auto operator<<(std::ostream& os, const Mahjong::Meld& meld) -> std::ostream& {
  os << "{ type: " << MeldTypeToStr(meld.type);
  os << ", start: " << meld.start.toStr() << "}";
  return os;
}
