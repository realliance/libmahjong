#include "player.h"

#include <string>

#include "playercontroller.h"

std::ostream& operator<<(std::ostream& os, const mahjong::Player& player) {
  os << "{ controller: "
     << ((player.controller != nullptr) ? player.controller->Name()
                                        : "NULLPTR");
  os << " points: " << player.points;
  return os;
}
