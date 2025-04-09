#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "analysis.h"
#include "handnode.h"
#include "piecetype.h"

namespace mahjong {

struct Breakdown {
  std::shared_ptr<Node> rootNode;
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

auto possibleChiForward(const std::array<int8_t, Piece::kPiecesize> counts,
                        Piece p) -> bool {
  if (p.isHonor()) {
    return false;
  }
  return counts.at((p).toUint8_t()) > 0 && counts.at((p + 1).toUint8_t()) > 0 &&
         counts.at((p + 2).toUint8_t()) > 0;
}

auto possibleChis(const std::array<int8_t, Piece::kPiecesize> counts, Piece p)
    -> int {
  return p.isHonor() ? 0
                     : ((static_cast<int>(possibleChiForward(counts, p)) +
                         static_cast<int>(possibleChiForward(counts, p - 1)) +
                         static_cast<int>(possibleChiForward(counts, p - 2))));
}

auto anyPossibleChi(const std::array<int8_t, Piece::kPiecesize> counts, Piece p)
    -> bool {
  return possibleChis(counts, p) > 0;
}

auto possiblePair(const std::array<int8_t, Piece::kPiecesize> counts, Piece p)
    -> bool {
  return (counts.at(p.toUint8_t()) == 2);
}

auto possiblePon(const std::array<int8_t, Piece::kPiecesize> counts, Piece p)
    -> bool {
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

auto addLeaf(Breakdown* b, Piece start, Node::Type type) -> Node* {
  std::vector<Node*> leaves;
  b->currentNode->leaves.push_back(new Node(
      b->id++,                       // id
      type,                          // type
      start,                         // Start
      b->currentNode,                // parent
      leaves,                        // leaves
      b->currentNode->leaves.size()  // leafPosInParent
      ));
  return b->currentNode->leaves.back();
}

auto breakdownForwardChi(Breakdown* b, int piecePos) -> void {
  Piece start = b->pieces[piecePos];
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

auto breakdownPon(Breakdown* b, int piecePos) -> void {
  Piece start = b->pieces[piecePos];
  b->counts.at(b->pieces[piecePos].toUint8_t()) -= 3;
  if (b->counts.at(b->pieces[piecePos].toUint8_t()) == 0) {
    b->pieces.erase(
        std::remove(b->pieces.begin(), b->pieces.end(), b->pieces[piecePos]),
        b->pieces.end());
  }
  b->currentNode = addLeaf(b, start, Node::kPonSet);
}

auto breakdownPair(Breakdown* b, int piecePos) -> void {
  b->paired = true;
  Piece start = b->pieces[piecePos];
  b->counts.at(b->pieces[piecePos].toUint8_t()) -= 2;
  if (b->counts.at(b->pieces[piecePos].toUint8_t()) == 0) {
    b->pieces.erase(
        std::remove(b->pieces.begin(), b->pieces.end(), b->pieces[piecePos]),
        b->pieces.end());
  }
  b->currentNode = addLeaf(b, start, Node::kPair);
}

auto breakdownSingle(Breakdown* b, int piecePos) -> void {
  b->currentNode = addLeaf(b, b->pieces[piecePos], Node::kSingle);

  b->counts.at(b->pieces[piecePos].toUint8_t())--;
  if (b->counts.at(b->pieces[piecePos].toUint8_t()) == 0) {
    b->pieces.erase(
        std::remove(b->pieces.begin(), b->pieces.end(), b->pieces[piecePos]),
        b->pieces.end());
  }
}

auto getNextPiece(Breakdown* b) -> int {
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

auto resetCounts(Breakdown* b, const Node* target) -> void {
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
auto driver(Breakdown* b) -> void {
  for (updatePossibilities(b); b->pieces.empty(); updatePossibilities(b)) {
    int piece_pos = getNextPiece(b);
    if (b->possibilities[piece_pos] == 0) {
      breakdownSingle(b, piece_pos);
      continue;
    }
    if (b->possibilities[piece_pos] == 1) {
      if (anyPossibleChi(b->counts, b->pieces[piece_pos])) {
        for (int i = 0; i < 3; i++) {
          if (possibleChiForward(b->counts, b->pieces[piece_pos] - i)) {
            breakdownForwardChi(b, piece_pos - i);
            break;
          }
        }
        continue;
      }
      if (possiblePon(b->counts, b->pieces[piece_pos])) {
        breakdownPon(b, piece_pos);
        continue;
      }
      if (possiblePair(b->counts, b->pieces[piece_pos])) {
        breakdownPair(b, piece_pos);
        continue;
      }
    }
    if (b->possibilities[piece_pos] == 2) {
      auto* current = b->currentNode;
      int branch = 0;
      if (possibleChiForward(b->counts, b->pieces[piece_pos] - 0)) {
        branch++;
        breakdownForwardChi(b, piece_pos - 0);
        driver(b);
        resetCounts(b, current);
      }
      if (possibleChiForward(b->counts, b->pieces[piece_pos] - 1)) {
        branch++;
        breakdownForwardChi(b, piece_pos - 1);
        if (branch == 2) {
          continue;
        }
        driver(b);
        resetCounts(b, current);
      }
      if (possibleChiForward(b->counts, b->pieces[piece_pos] - 2)) {
        branch++;
        breakdownForwardChi(b, piece_pos - 2);
        if (branch == 2) {
          continue;
        }
        driver(b);
        resetCounts(b, current);
      }
      if (possiblePon(b->counts, b->pieces[piece_pos])) {
        branch++;
        breakdownPon(b, piece_pos);
        if (branch == 2) {
          continue;
        }
        driver(b);
        resetCounts(b, current);
      }
      if (possiblePair(b->counts, b->pieces[piece_pos])) {
        branch++;
        breakdownPair(b, piece_pos);
        if (branch == 2) {
          continue;
        }
      }
    }
  }
}

}  // namespace

auto breakdownHand(std::vector<Piece> pieces) -> std::shared_ptr<Node> {
  Breakdown b;
  b.pieces = std::move(pieces);
  countPieces(&b);
  std::sort(b.pieces.begin(), b.pieces.end());
  b.pieces.erase(std::unique(b.pieces.begin(), b.pieces.end()), b.pieces.end());

  b.rootNode = std::make_shared<Node>(Node(b.id++,                      // id
                                           Node::kRoot,                 // type
                                           Piece(Piece::Type::kError),  // Start
                                           {},  // parent
                                           {},  // leaves
                                           0    // leafPosInParent
                                           ));
  b.currentNode = b.rootNode.get();

  driver(&b);

  return b.rootNode;
}
}  // namespace mahjong
