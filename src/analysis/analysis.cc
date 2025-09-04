#include "analysis/analysis.h"

#include <algorithm>
#include <memory>
#include <span>
#include <utility>
#include <vector>

#include "analysis/handnode.h"
#include "analysis/handtree.h"
#include "types/gamestate.h"
#include "types/meld.h"
#include "types/piecetype.h"
#include "types/sets.h"

namespace mahjong {
namespace {
void countPieces(Breakdown* b) {
  for (const auto& p : b->pieces) {
    b->counts[p]++;
  }
}
}  // namespace

std::vector<Branch> AnalyzeHand(const Hand& hand, const bool only_complete) {
  Branch base_branch;
  base_branch.open = !hand.melds.empty();
  for (const auto& meld : hand.melds_range()) {
    switch (meld.type) {
      case SetType::kChi:
        base_branch.chis.emplace_back(meld.start);
        break;
      case SetType::kPon:
        base_branch.pons.emplace_back(meld.start);
        break;
      case SetType::kConcealedKan:
      case SetType::kKan:
        base_branch.kans.emplace_back(meld.start);
        break;
      default:
        break;
    }
  }
  std::vector<Branch> branches;
  const std::unique_ptr<Node> root = breakdownHand(hand.live_range());
  for (const auto& branch_vec : Node::AsBranchVectors(root.get())) {
    branches.push_back(base_branch);
    Branch& branch = branches.back();
    for (const Node* node : branch_vec) {
      switch (node->type()) {
        case SetType::kChi:
          branch.chis.emplace_back(node->start());
          break;
        case SetType::kPon:
          branch.pons.emplace_back(node->start());
          break;
        case SetType::kConcealedKan:
        case SetType::kKan:
          branch.kans.emplace_back(node->start());
          break;
        case SetType::kPair:
          branch.pairs.emplace_back(node->start());
          break;
        case SetType::kSingle:
          branch.singles.emplace_back(node->start());
          break;
      }
    }
    branch.complete = branch.singles.empty();
    if (only_complete && !branch.complete) {
      branches.pop_back();
    }
  }
  return branches;
}

std::unique_ptr<Node> breakdownHand(const std::span<const Piece>& pieces) {
  Breakdown b;
  b.rootNode = std::make_unique<Node>(/*id=*/b.id++);
  b.currentNode = b.rootNode.get();
  b.pieces = std::vector<Piece>(pieces.begin(), pieces.end());
  countPieces(&b);
  std::ranges::sort(b.pieces);
  auto [begin, end] = std::ranges::unique(b.pieces);
  b.pieces.erase(begin, end);

  driver(&b);

  return std::move(b.rootNode);
}

}  // namespace mahjong
