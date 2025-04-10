#pragma once
#include <string>

namespace mahjong {

enum Wind { kEast, kSouth, kWest, kNorth };

inline std::string WindToStr(Wind s) {
  switch (s) {
    case kEast:
      return "East";
    case kSouth:
      return "South";
    case kWest:
      return "West";
    case kNorth:
      return "North";
    default:
      return "InvalidWind";
  }
}

}  // namespace mahjong
