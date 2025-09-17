#pragma once
#include <memory>
#include <vector>
#include <span>

#include "analysis/handnode.h"
#include "types/hand.h"
#include "types/piecetype.h"

namespace mahjong {
struct Branch {
  std::vector<Piece> chis;
  std::vector<Piece> pons;
  std::vector<Piece> kans;
  std::vector<Piece> pairs;
  std::vector<Piece> singles;
  bool open = true;
  bool complete = true;
};

std::unique_ptr<Node> breakdownHand(const std::span<const Piece>& pieces);
std::vector<Branch> AnalyzeHand(const Hand& hand, bool only_complete = false);
}  // namespace mahjong
