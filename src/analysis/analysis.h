#pragma once
#include <memory>
#include <vector>

#include "types/handnode.h"
#include "types/piecetype.h"

namespace mahjong {

std::vector<Piece> GetPossibleStdFormHand();
std::vector<Piece> GetPossibleTenpaiHand(bool replacement = false);
bool TestStdForm(const std::vector<Piece>& hand);
std::unique_ptr<Node> breakdownHand(const std::vector<Piece>& pieces);

}  // namespace mahjong
