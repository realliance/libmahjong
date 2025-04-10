#pragma once

#include <string>
#include <vector>

namespace mahjong {
class Piece;
std::vector<Piece> HandFromNotation(const std::string& notation);
bool IsValidNotation(const std::string& notation);
std::string HandToNotation(const std::vector<Piece>& hand);
}  // namespace mahjong
