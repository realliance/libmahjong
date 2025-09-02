#pragma once

#include <map>
#include "analysis/handnode.h"
#include "types/piecetype.h"
namespace mahjong {
struct Breakdown {
  std::unique_ptr<Node> rootNode;
  Node* currentNode{};
  int id = 0;
  std::map<Piece, int> counts;
  std::map<Piece, int> possibilities;
  std::vector<Piece> pieces;
};

void driver(Breakdown* b);

}  // namespace mahjong
