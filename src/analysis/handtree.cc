#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "analysis.h"
#include "types/handnode.h"
#include "types/piecetype.h"

namespace mahjong {

struct Breakdown {
  std::unique_ptr<Node> rootNode;
  Node* currentNode{};
  bool paired = false;
  int minPossible{};
  int id = 0;
  std::array<int8_t, Piece::kPiecesize> counts = {};
  std::vector<int> possibilities;
  std::vector<Piece> pieces;
};

namespace {

const int kMaxPossible = 14;

bool possibleChiForward(const std::array<int8_t, Piece::kPiecesize> counts,
                        Piece p) {
  if (p.isHonor()) {
    return false;
  }
  return counts.at((p).toUint8_t()) > 0 && counts.at((p + 1).toUint8_t()) > 0 &&
         counts.at((p + 2).toUint8_t()) > 0;
}

int possibleChis(const std::array<int8_t, Piece::kPiecesize> counts, Piece p) {
  return p.isHonor() ? 0
                     : ((static_cast<int>(possibleChiForward(counts, p)) +
                         static_cast<int>(possibleChiForward(counts, p - 1)) +
                         static_cast<int>(possibleChiForward(counts, p - 2))));
}

bool anyPossibleChi(const std::array<int8_t, Piece::kPiecesize> counts,
                    Piece p) {
  return possibleChis(counts, p) > 0;
}

bool possiblePair(const std::array<int8_t, Piece::kPiecesize> counts, Piece p) {
  return (counts.at(p.toUint8_t()) == 2);
}

bool possiblePon(const std::array<int8_t, Piece::kPiecesize> counts, Piece p) {
  return counts.at(p.toUint8_t()) == 3;
}

void countPieces(Breakdown* b) {
  for (const auto& p : b->pieces) {
    b->counts.at(p.toUint8_t())++;
  }
}

void updatePossibilities(Breakdown* b) {
  b->possibilities.resize(b->pieces.size());
  b->minPossible = kMaxPossible;
  for (size_t i = 0; i < b->pieces.size(); i++) {
    b->possibilities.at(i) = 0;
    b->possibilities.at(i) += possibleChis(b->counts, b->pieces.at(i));
    b->possibilities.at(i) +=
        b->paired ? 0
                  : static_cast<int>(possiblePair(b->counts, b->pieces.at(i)));
    b->possibilities.at(i) +=
        static_cast<int>(possiblePon(b->counts, b->pieces.at(i)));
    b->minPossible = b->possibilities.at(i) < b->minPossible
                         ? b->possibilities.at(i)
                         : b->minPossible;
  }
}

Node* addLeaf(Breakdown* b, Piece start, Node::Type type) {
  b->currentNode->leaves.push_back(std::make_unique<Node>(
      b->id++,                       // id
      type,                          // type
      start,                         // Start
      b->currentNode,                // parent
      b->currentNode->leaves.size()  // leafPosInParent
      ));
  return b->currentNode->leaves.back().get();
}

void breakdownForwardChi(Breakdown* b, int piecePos) {
  const Piece start = b->pieces[piecePos];
  for (int i = 0; i < 3; i++) {
    b->counts.at((start + i).toUint8_t())--;
    if (b->counts.at((start + i).toUint8_t()) == 0) {
      b->pieces.erase(
          std::remove(b->pieces.begin(), b->pieces.end(), (start + i)),
          b->pieces.end());
    }
  }
  b->currentNode = addLeaf(b, start, Node::kChiSet);
}

void breakdownPon(Breakdown* b, int piecePos) {
  const Piece start = b->pieces[piecePos];
  b->counts.at(b->pieces[piecePos].toUint8_t()) -= 3;
  if (b->counts.at(b->pieces[piecePos].toUint8_t()) == 0) {
    b->pieces.erase(
        std::remove(b->pieces.begin(), b->pieces.end(), b->pieces[piecePos]),
        b->pieces.end());
  }
  b->currentNode = addLeaf(b, start, Node::kPonSet);
}

void breakdownPair(Breakdown* b, int piecePos) {
  b->paired = true;
  const Piece start = b->pieces[piecePos];
  b->counts.at(b->pieces[piecePos].toUint8_t()) -= 2;
  if (b->counts.at(b->pieces[piecePos].toUint8_t()) == 0) {
    b->pieces.erase(
        std::remove(b->pieces.begin(), b->pieces.end(), b->pieces[piecePos]),
        b->pieces.end());
  }
  b->currentNode = addLeaf(b, start, Node::kPair);
}

void breakdownSingle(Breakdown* b, int piecePos) {
  b->currentNode = addLeaf(b, b->pieces[piecePos], Node::kSingle);

  b->counts.at(b->pieces[piecePos].toUint8_t())--;
  if (b->counts.at(b->pieces[piecePos].toUint8_t()) == 0) {
    b->pieces.erase(
        std::remove(b->pieces.begin(), b->pieces.end(), b->pieces[piecePos]),
        b->pieces.end());
  }
}

int getNextPiece(Breakdown* b) {
  int piece_pos = 0;
  for (size_t i = 0; i < b->pieces.size(); i++) {
    if (b->possibilities.at(i) <= b->possibilities[piece_pos]) {
      piece_pos = i;
      if (b->possibilities.at(i) == b->minPossible) {
        break;
      }
    }
  }
  return piece_pos;
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
    if (b->currentNode->parent == nullptr) {
      std::cerr << "reset Failure: parent node nullptr." << '\n';
      std::ofstream os("error.gv");
      b->rootNode->DumpAsDot(os);
      os.close();
      throw -2;
    }
    if (b->currentNode->parent->type == Node::kError) {
      std::cerr << "reset Failure: reset up to an error." << '\n';
      std::ofstream os("error.gv");
      b->rootNode->DumpAsDot(os);
      os.close();
      throw -4;
    }
    if (b->currentNode->type == Node::kChiSet) {
      for (int i = 0; i < 3; i++) {
        if (b->counts.at((b->currentNode->start + i).toUint8_t()) == 0) {
          b->pieces.push_back(Piece{b->currentNode->start + i});
          std::sort(b->pieces.begin(), b->pieces.end());
        }
        b->counts.at((b->currentNode->start + i).toUint8_t())++;
      }
    } else {
      if (b->counts.at(b->currentNode->start.toUint8_t()) == 0) {
        b->pieces.emplace_back(b->currentNode->start);
        std::sort(b->pieces.begin(), b->pieces.end());
      }
      if (b->currentNode->type == Node::kSingle) {
        b->counts.at(b->currentNode->start.toUint8_t())++;
      }
      if (b->currentNode->type == Node::kPair) {
        b->paired = false;
        b->counts.at(b->currentNode->start.toUint8_t()) += 2;
      }
      if (b->currentNode->type == Node::kPonSet) {
        b->counts.at(b->currentNode->start.toUint8_t()) += 3;
      }
    }
    b->currentNode = b->currentNode->parent;
  }
}

// NOLINTNEXTLINE(misc-no-recursion)
void driver(Breakdown* b) {
  // - Always process the tile into the fewest groupings first
  // - If a tile has only one way to be grouped, use it
  // - If a tile has multiple ways, try each possibility
  // - Continue until all tiles are in groups

  // While there are ungrouped pieces
  for (updatePossibilities(b); !b->pieces.empty(); updatePossibilities(b)) {
    // Get the piece with minimum possibilities
    const int piece_pos = getNextPiece(b);

    if (b->possibilities[piece_pos] == 0) {
      // No valid grouping possible, single piece
      breakdownSingle(b, piece_pos);
      continue;
    }

    if (b->possibilities[piece_pos] == 1) {
      // One way to be grouped, use it
      if (anyPossibleChi(b->counts, b->pieces[piece_pos])) {
        // A chi is possible, determine how it can be a part of one
        for (int i = 0; i < 3; i++) {
          const Piece chi_start = b->pieces[piece_pos] - i;
          // Check if we can start a chi with this piece
          if (possibleChiForward(b->counts, chi_start)) {
            // Find the position of chi_start in the pieces vector
            auto it = std::find(b->pieces.begin(), b->pieces.end(), chi_start);
            if (it != b->pieces.end()) {
              const int chi_start_pos = std::distance(b->pieces.begin(), it);
              breakdownForwardChi(b, chi_start_pos);
            }
            break;
          }
        }
        continue;
      }
      if (possiblePon(b->counts, b->pieces[piece_pos])) {
        // A pon is possible
        breakdownPon(b, piece_pos);
        continue;
      }
      if (possiblePair(b->counts, b->pieces[piece_pos])) {
        // A pair is possible
        breakdownPair(b, piece_pos);
        continue;
      }
    }
    if (b->possibilities[piece_pos] >= 2) {
      // Tile has multiple groups, we need to check branches

      // Save current position in tree so we can backtrack
      auto* current = b->currentNode;

      // Check grouping possibilities
      const bool can_chi_start =
          possibleChiForward(b->counts, b->pieces[piece_pos] - 0);

      bool can_chi_middle = false;
      int chi_middle_pos = -1;
      if (possibleChiForward(b->counts, b->pieces[piece_pos] - 1)) {
        const Piece chi_start = b->pieces[piece_pos] - 1;
        auto it = std::find(b->pieces.begin(), b->pieces.end(), chi_start);
        if (it != b->pieces.end()) {
          can_chi_middle = true;
          chi_middle_pos = std::distance(b->pieces.begin(), it);
        }
      }

      bool can_chi_end = false;
      int chi_end_pos = -1;
      if (possibleChiForward(b->counts, b->pieces[piece_pos] - 2)) {
        const Piece chi_start = b->pieces[piece_pos] - 2;
        auto it = std::find(b->pieces.begin(), b->pieces.end(), chi_start);
        if (it != b->pieces.end()) {
          can_chi_end = true;
          chi_end_pos = std::distance(b->pieces.begin(), it);
        }
      }

      const bool can_pon = possiblePon(b->counts, b->pieces[piece_pos]);
      const bool can_pair = possiblePair(b->counts, b->pieces[piece_pos]);

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
        breakdownForwardChi(b, piece_pos);
        driver(b);
        if (current_branch < total_branches) {
          resetCounts(b, current);
        }
      }

      if (can_chi_middle) {
        // Can be the middle of a chi
        current_branch++;
        breakdownForwardChi(b, chi_middle_pos);
        driver(b);
        if (current_branch < total_branches) {
          resetCounts(b, current);
        }
      }

      if (can_chi_end) {
        // Can be the end of a chi
        current_branch++;
        breakdownForwardChi(b, chi_end_pos);
        driver(b);
        if (current_branch < total_branches) {
          resetCounts(b, current);
        }
      }

      if (can_pon) {
        // Can be a pon
        current_branch++;
        breakdownPon(b, piece_pos);
        driver(b);
        if (current_branch < total_branches) {
          resetCounts(b, current);
        }
      }

      if (can_pair) {
        // Can be a pair
        current_branch++;
        breakdownPair(b, piece_pos);
        driver(b);
        if (current_branch < total_branches) {
          resetCounts(b, current);
        }
      }
    }
  }
}

}  // namespace

std::unique_ptr<Node> breakdownHand(const std::vector<Piece>& pieces) {
  Breakdown b;
  b.rootNode = std::make_unique<Node>(b.id++,                     // id
                                      Node::kRoot,                // type
                                      Piece(Piece::Type::kError)  // Start
  );
  b.currentNode = b.rootNode.get();
  b.pieces = pieces;
  countPieces(&b);
  std::sort(b.pieces.begin(), b.pieces.end());
  b.pieces.erase(std::unique(b.pieces.begin(), b.pieces.end()), b.pieces.end());

  driver(&b);

  return std::move(b.rootNode);
}
}  // namespace mahjong
