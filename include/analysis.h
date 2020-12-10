#pragma once
#include <memory>
#include <vector>

#include "handnode.h"
#include "piecetype.h"

namespace Mahjong {

auto GetPossibleStdFormHand() -> std::vector<Piece>;
auto GetPossibleTenpaiHand(bool replacement = false, Piece* removed = nullptr) -> std::vector<Piece>;
auto TestStdForm(std::vector<Piece> hand) -> bool;
auto breakdownHand(std::vector<Piece> pieces) -> std::shared_ptr<Node>;

}  // namespace Mahjong
