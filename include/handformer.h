#pragma once

#include <string>
#include <vector>

#include "piecetype.h"

namespace Mahjong {
auto HandFromNotation(std::string notation) -> std::vector<Piece>;
}