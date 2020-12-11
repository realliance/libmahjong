#pragma once

#include <string>
#include <vector>

namespace Mahjong {
class Piece;
auto HandFromNotation(std::string notation) -> std::vector<Piece>;
auto IsValidNotation(std::string notation) -> bool;
auto HandToNotation(std::vector<Piece> hand) -> std::string;
}  // namespace Mahjong
