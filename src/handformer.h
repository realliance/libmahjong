#pragma once

#include <string>
#include <vector>

namespace mahjong {
class Piece;
auto HandFromNotation(const std::string& notation) -> std::vector<Piece>;
auto IsValidNotation(const std::string& notation) -> bool;
auto HandToNotation(const std::vector<Piece>& hand) -> std::string;
}  // namespace mahjong
