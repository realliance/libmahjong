#pragma once

#include <string>
#include <vector>

#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"

namespace mahjong {

std::vector<Piece> HandFromNotation(const std::string& notation);
void HandFromNotation(const std::string& notation, Hand* hand);
bool IsValidNotation(const std::string& notation);
std::string PlayerToNotation(const std::vector<Piece>& hand);

}  // namespace mahjong
