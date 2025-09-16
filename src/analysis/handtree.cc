#include "analysis/handtree.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "analysis/handnode.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"

namespace mahjong {
namespace {

const int kMaxPossible = 14;
bool possibleChiForward(const std::map<Piece, int>& counts, Piece p) {
  if (p.isHonor() || !counts.contains(p) || !counts.contains(p + 1) ||
      !counts.contains(p + 2)) {
    return false;
  }
  return counts.at(p) > 0 && counts.at(p + 1) > 0 && counts.at(p + 2) > 0;
}

int possibleChis(const std::map<Piece, int>& counts, Piece p) {
  int chi_count = possibleChiForward(counts, p) ? 1 : 0;
  chi_count += possibleChiForward(counts, p - 1) ? 1 : 0;
  chi_count += possibleChiForward(counts, p - 2) ? 1 : 0;
  return chi_count;
}

bool anyPossibleChi(const std::map<Piece, int>& counts, Piece p) {
  return possibleChis(counts, p) > 0;
}

bool possiblePair(const std::map<Piece, int>& counts, Piece p) {
  return counts.contains(p) && counts.at(p) >= 2;
}

bool possiblePon(const std::map<Piece, int>& counts, Piece p) {
  return counts.contains(p) && counts.at(p) >= 3;
}

Piece updatePossibilities(Breakdown* b) {
  b->possibilities.clear();
  int min_possible = kMaxPossible;
  Piece min_possible_piece = kError;
  for (const auto& piece : b->pieces) {
    b->possibilities[piece] += possibleChis(b->counts, piece);
    if (possiblePair(b->counts, piece)) {
      b->possibilities[piece]++;
    }
    if (possiblePon(b->counts, piece)) {
      b->possibilities[piece]++;
    }
    if (b->possibilities[piece] < min_possible) {
      min_possible = b->possibilities[piece];
      min_possible_piece = piece;
    }
  }
  return min_possible_piece;
}

void breakdownForwardChi(Breakdown* b, Piece piece) {
  for (int i = 0; i < 3; i++) {
    b->counts[piece + i]--;
    if (b->counts[piece + i] == 0) {
      b->pieces.erase(
          std::remove(b->pieces.begin(), b->pieces.end(), piece + i),
          b->pieces.end());
    }
  }
  b->currentNode = b->currentNode->addLeaf(piece, SetType::kChi, b->id++);
}

void breakdownPon(Breakdown* b, Piece piece) {
  b->counts[piece] -= 3;
  if (b->counts[piece] == 0) {
    b->pieces.erase(std::remove(b->pieces.begin(), b->pieces.end(), piece),
                    b->pieces.end());
  }
  b->currentNode = b->currentNode->addLeaf(piece, SetType::kPon, b->id++);
}

void breakdownPair(Breakdown* b, Piece piece) {
  b->counts[piece] -= 2;
  if (b->counts[piece] == 0) {
    b->pieces.erase(std::remove(b->pieces.begin(), b->pieces.end(), piece),
                    b->pieces.end());
  }
  b->currentNode = b->currentNode->addLeaf(piece, SetType::kPair, b->id++);
}

void breakdownSingle(Breakdown* b, Piece piece) {
  b->currentNode = b->currentNode->addLeaf(piece, SetType::kSingle, b->id++);

  b->counts[piece]--;
  if (b->counts[piece] == 0) {
    b->pieces.erase(std::remove(b->pieces.begin(), b->pieces.end(), piece),
                    b->pieces.end());
  }
}

void resetCounts(Breakdown* b, const Node* target) {
  if (b->currentNode == nullptr) {
    std::cerr << "reset Failure: current node nullptr." << '\n';
    std::ofstream os("error.gv");
    b->rootNode->DumpAsDot(os);
    os.close();
    throw -1;
  }
  while (b->currentNode != target) {
    if (b->currentNode->parent() == nullptr) {
      std::cerr << "reset Failure: parent node nullptr." << '\n';
      std::ofstream os("error.gv");
      b->rootNode->DumpAsDot(os);
      os.close();
      throw -2;
    }
    if (b->currentNode->type() == SetType::kChi) {
      for (int i = 0; i < 3; i++) {
        if (b->counts[b->currentNode->start() + i] == 0) {
          b->pieces.push_back(Piece{b->currentNode->start() + i});
          std::sort(b->pieces.begin(), b->pieces.end());
        }
        b->counts[b->currentNode->start() + i]++;
      }
    } else {
      if (b->counts[b->currentNode->start()] == 0) {
        b->pieces.emplace_back(b->currentNode->start());
        std::sort(b->pieces.begin(), b->pieces.end());
      }
      if (b->currentNode->type() == SetType::kSingle) {
        b->counts[b->currentNode->start()]++;
      }
      if (b->currentNode->type() == SetType::kPair) {
        b->counts[b->currentNode->start()] += 2;
      }
      if (b->currentNode->type() == SetType::kPon) {
        b->counts[b->currentNode->start()] += 3;
      }
    }
    b->currentNode = b->currentNode->parent();
  }
}
}  // namespace

// NOLINTNEXTLINE(misc-no-recursion)
void driver(Breakdown* b) {
  // - Always process the tile into the fewest groupings first
  // - If a tile has only one way to be grouped, use it
  // - If a tile has multiple ways, try each possibility
  // - Continue until all tiles are in groups

  // While there are ungrouped pieces, get the piece with minimum possibilities
  for (Piece current_piece = updatePossibilities(b); !b->pieces.empty();
       current_piece = updatePossibilities(b)) {
    if (b->possibilities[current_piece] == 0) {
      // No valid grouping possible, single piece
      breakdownSingle(b, current_piece);
      continue;
    }

    if (b->possibilities[current_piece] == 1) {
      // One way to be grouped, use it
      if (anyPossibleChi(b->counts, current_piece)) {
        // A chi is possible, determine how it can be a part of one
        for (int i = 0; i < 3; i++) {
          const Piece chi_start = current_piece - i;
          // Check if we can start a chi with this piece
          if (possibleChiForward(b->counts, chi_start)) {
            // Find the position of chi_start in the pieces vector
            auto piece_itr =
                std::find(b->pieces.begin(), b->pieces.end(), chi_start);
            if (piece_itr != b->pieces.end()) {
              breakdownForwardChi(b, *piece_itr);
            }
            break;
          }
        }
        continue;
      }
      if (possiblePon(b->counts, current_piece)) {
        // A pon is possible
        breakdownPon(b, current_piece);
        continue;
      }
      if (possiblePair(b->counts, current_piece)) {
        // A pair is possible
        breakdownPair(b, current_piece);
        continue;
      }
    }
    if (b->possibilities[current_piece] >= 2) {
      // Tile has multiple groups, we need to check branches

      // Save current position in tree so we can backtrack
      auto* current = b->currentNode;

      // Check grouping possibilities
      const bool can_chi_start = possibleChiForward(b->counts, current_piece);

      bool can_chi_middle = false;
      Piece chi_middle_piece;
      if (possibleChiForward(b->counts, current_piece - 1)) {
        const Piece chi_start = current_piece - 1;
        auto it = std::find(b->pieces.begin(), b->pieces.end(), chi_start);
        if (it != b->pieces.end()) {
          can_chi_middle = true;
          chi_middle_piece = *it;
        }
      }

      bool can_chi_end = false;
      Piece chi_end_piece;
      if (possibleChiForward(b->counts, current_piece - 2)) {
        const Piece chi_start = current_piece - 2;
        auto it = std::find(b->pieces.begin(), b->pieces.end(), chi_start);
        if (it != b->pieces.end()) {
          can_chi_end = true;
          chi_end_piece = *it;
        }
      }

      const bool can_pon = possiblePon(b->counts, current_piece);
      const bool can_pair = possiblePair(b->counts, current_piece);

      // Count total branches that will actually execute
      int total_branches = 0;
      if (can_chi_start) {
        total_branches++;
      }
      if (can_chi_middle) {
        total_branches++;
      }
      if (can_chi_end) {
        total_branches++;
      }
      if (can_pon) {
        total_branches++;
      }
      if (can_pair) {
        total_branches++;
      }

      // Execute branches
      int current_branch = 0;

      if (can_chi_start) {
        // Can be the beginning of a chi
        current_branch++;
        breakdownForwardChi(b, current_piece);
        driver(b);
        if (current_branch < total_branches) {
          resetCounts(b, current);
        }
      }

      if (can_chi_middle) {
        // Can be the middle of a chi
        current_branch++;
        breakdownForwardChi(b, chi_middle_piece);
        driver(b);
        if (current_branch < total_branches) {
          resetCounts(b, current);
        }
      }

      if (can_chi_end) {
        // Can be the end of a chi
        current_branch++;
        breakdownForwardChi(b, chi_end_piece);
        driver(b);
        if (current_branch < total_branches) {
          resetCounts(b, current);
        }
      }

      if (can_pon) {
        // Can be a pon
        current_branch++;
        breakdownPon(b, current_piece);
        driver(b);
        if (current_branch < total_branches) {
          resetCounts(b, current);
        }
      }

      if (can_pair) {
        // Can be a pair
        current_branch++;
        breakdownPair(b, current_piece);
        driver(b);
        if (current_branch < total_branches) {
          resetCounts(b, current);
        }
      }
    }
  }
}
}  // namespace mahjong
