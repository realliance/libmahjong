#include "player.h"

#include <string>

#include "playercontroller.h"

auto operator<<(std::ostream& os, const mahjong::Player& player)
    -> std::ostream& {
  os << "{ controller: "
     << ((player.controller != nullptr) ? player.controller->Name()
                                        : "NULLPTR");
  os << " points: " << player.points;
  return os;
}
