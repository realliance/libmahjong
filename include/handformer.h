#pragma once

#include <string>
#include <vector>

namespace Mahjong {
class Piece;
auto HandFromNotation(std::string notation) -> std::vector<Piece>;
}  // namespace Mahjong
