#pragma once
#include <memory>
#include <vector>

#include "types/handnode.h"
#include "types/piecetype.h"

namespace mahjong {

std::vector<Piece> GetPossibleStdFormHand();
std::vector<Piece> GetPossibleTenpaiHand(bool replacement = false);
bool TestStdForm(std::vector<Piece> hand);
std::shared_ptr<Node> breakdownHand(std::vector<Piece> pieces);

}  // namespace mahjong
