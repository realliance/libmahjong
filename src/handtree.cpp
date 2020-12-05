#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <ext/alloc_traits.h>
#include <fstream>  // IWYU pragma: keep
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "analysis.h"
#include "handnode.h"
#include "piecetype.h"

using Mahjong::Node, Mahjong::Piece;

struct Breakdown {
  std::shared_ptr<Node> rootNode{};
  Node* currentNode{};
  bool paired = false;
  int maxPossible{};
  int id = 0;
  std::array<int8_t, Piece::PIECESIZE> counts = {};
  std::vector<int> possibilities;
  std::vector<Piece> pieces;
};

auto possibleChiForward(const std::array<int8_t, Piece::PIECESIZE> counts, Piece p) -> bool {
  if (p.isHonor()) {
    return false;
  }
  return counts.at((p).toUint8_t()) > 0 && counts.at((p + 1).toUint8_t()) > 0 && counts.at((p + 2).toUint8_t()) > 0;
}

auto possibleChis(const std::array<int8_t, Piece::PIECESIZE> counts, Piece p) -> int {
  return p.isHonor() ? 0 : ((static_cast<int>(possibleChiForward(counts, p)) + static_cast<int>(possibleChiForward(counts, p - 1)) + static_cast<int>(possibleChiForward(counts, p - 2))));
}

auto anyPossibleChi(const std::array<int8_t, Piece::PIECESIZE> counts, Piece p) -> bool {
  return possibleChis(counts, p) > 0;
}

auto possiblePair(const std::array<int8_t, Piece::PIECESIZE> counts, Piece p) -> bool {
  return (counts.at(p.toUint8_t()) >= 2);
}

auto possiblePon(const std::array<int8_t, Piece::PIECESIZE> counts, Piece p) -> bool {
  return counts.at(p.toUint8_t()) >= 3;
}

void countPieces(Breakdown* b) {
  for (const auto& p : b->pieces) {
    b->counts.at(p.toUint8_t())++;
  }
}

void updatePossibilities(Breakdown* b) {
  b->possibilities.resize(b->pieces.size());
  b->maxPossible = 0;
  for (size_t i = 0; i < b->pieces.size(); i++) {
    b->possibilities.at(i) = 0;
    b->possibilities.at(i) += possibleChis(b->counts, b->pieces.at(i));
    b->possibilities.at(i) += b->paired ? 0 : static_cast<int>(possiblePair(b->counts, b->pieces.at(i)));
    b->possibilities.at(i) += static_cast<int>(possiblePon(b->counts, b->pieces.at(i)));
    b->maxPossible = b->possibilities.at(i) > b->maxPossible ? b->possibilities.at(i) : b->maxPossible;
  }
}

auto addLeaf(Breakdown* b, Piece start, Node::Type type) -> Node* {
  std::vector<Node*> leaves;
  b->currentNode->leaves.push_back(new Node(
    b->id++,                       //id
    type,                          //type
    start,                         //Start
    b->currentNode,                //parent
    leaves,                        //leaves
    b->currentNode->leaves.size()  //leafPosInParent
    ));
  return b->currentNode->leaves.back();
}

auto breakdownForwardChi(Breakdown* b, int piecePos) -> void {
  Piece start = b->pieces[piecePos];
  for (int i = 0; i < 3; i++) {
    b->counts.at((start + i).toUint8_t())--;
    if (b->counts.at((start + i).toUint8_t()) == 0) {
      b->pieces.erase(std::remove(b->pieces.begin(), b->pieces.end(), (start + i)), b->pieces.end());
    }
  }
  b->currentNode = addLeaf(b, start, Node::ChiSet);
}

auto breakdownPon(Breakdown* b, int piecePos) -> void {
  Piece start = b->pieces[piecePos];
  b->counts.at(b->pieces[piecePos].toUint8_t()) -= 3;
  if (b->counts.at(b->pieces[piecePos].toUint8_t()) == 0) {
    b->pieces.erase(std::remove(b->pieces.begin(), b->pieces.end(), b->pieces[piecePos]), b->pieces.end());
  }
  b->currentNode = addLeaf(b, start, Node::PonSet);
  ;
}

auto breakdownPair(Breakdown* b, int piecePos) -> void {
  b->paired = true;
  Piece start = b->pieces[piecePos];
  b->counts.at(b->pieces[piecePos].toUint8_t()) -= 2;
  if (b->counts.at(b->pieces[piecePos].toUint8_t()) == 0) {
    b->pieces.erase(std::remove(b->pieces.begin(), b->pieces.end(), b->pieces[piecePos]), b->pieces.end());
  }
  b->currentNode = addLeaf(b, start, Node::Pair);
  ;
}

auto breakdownSingle(Breakdown* b, int piecePos) -> void {
  b->currentNode = addLeaf(b, b->pieces[piecePos], Node::Single);
  ;
  b->counts.at(b->pieces[piecePos].toUint8_t())--;
  if (b->counts.at(b->pieces[piecePos].toUint8_t()) == 0) {
    b->pieces.erase(std::remove(b->pieces.begin(), b->pieces.end(), b->pieces[piecePos]), b->pieces.end());
  }
}

auto getNextPiece(Breakdown* b) -> int {
  int piecePos = 0;
  for (size_t i = 0; i < b->pieces.size(); i++) {
    if (b->possibilities.at(i) >= b->possibilities[piecePos]) {
      piecePos = i;
      if (b->possibilities.at(i) == b->maxPossible) {
        break;
      }
    }
  }
  return piecePos;
}

auto resetCounts(Breakdown* b, const Node* target) -> void {
  if (b->currentNode == nullptr) {
    std::cerr << "reset Failure: current node nullptr." << std::endl;
    std::ofstream os("error.gv");
    b->rootNode->DumpAsDot(os);
    os.close();
    throw -1;
  }
  while (b->currentNode != target) {
    if (b->currentNode->parent == nullptr) {
      std::cerr << "reset Failure: parent node nullptr." << std::endl;
      std::ofstream os("error.gv");
      b->rootNode->DumpAsDot(os);
      os.close();
      throw -2;
    }
    if (b->currentNode->parent->type == Node::Error) {
      std::cerr << "reset Failure: reset up to an error." << std::endl;
      std::ofstream os("error.gv");
      b->rootNode->DumpAsDot(os);
      os.close();
      throw -4;
    }
    if (b->currentNode->type == Node::ChiSet) {
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
      if (b->currentNode->type == Node::Single) {
        b->counts.at(b->currentNode->start.toUint8_t())++;
      }
      if (b->currentNode->type == Node::Pair) {
        b->paired = false;
        b->counts.at(b->currentNode->start.toUint8_t()) += 2;
      }
      if (b->currentNode->type == Node::PonSet) {
        b->counts.at(b->currentNode->start.toUint8_t()) += 3;
      }
    }
    b->currentNode = b->currentNode->parent;
  }
}

auto driver(Breakdown* b) -> void {
  updatePossibilities(b);
  if (b->pieces.empty()) {
    return;
  }
  int piecePos = getNextPiece(b);
  if (b->possibilities[piecePos] == 0) {
    breakdownSingle(b, piecePos);
    return driver(b);
  }

  using possibleFunc = auto (*)(const std::array<int8_t, Piece::PIECESIZE>, Mahjong::Piece)->bool;
  using breakdownFunc = auto (*)(Breakdown*, int)->void;
  const std::vector<std::pair<possibleFunc, breakdownFunc>> possibilityandbreakdown = {
    {possibleChiForward, breakdownForwardChi},
    {[](auto counts, auto piece) { return possibleChiForward(counts, piece - 1); },
     [](auto counts, auto piece) { return breakdownForwardChi(counts, piece - 1); }},
    {[](auto counts, auto piece) { return possibleChiForward(counts, piece - 2); },
     [](auto counts, auto piece) { return breakdownForwardChi(counts, piece - 2); }},
    {possiblePon, breakdownPon},
    {possiblePair, breakdownPair}};
  int branch = 0;
  auto* current = b->currentNode;
  for (const auto& [possibility, breakdown] : possibilityandbreakdown) {
    if (possibility(b->counts, b->pieces[piecePos])) {
      breakdown(b, piecePos);
      driver(b);
      branch++;
      resetCounts(b, current);
      updatePossibilities(b);
      if (branch == b->possibilities[piecePos]) {
        break;
      }
    }
  }
}

auto Mahjong::breakdownHand(std::vector<Piece> _pieces) -> std::shared_ptr<Node> {
  Breakdown b;
  b.pieces = std::move(_pieces);
  countPieces(&b);
  std::sort(b.pieces.begin(), b.pieces.end());
  b.pieces.erase(std::unique(b.pieces.begin(), b.pieces.end()), b.pieces.end());

  b.rootNode = std::make_shared<Node>(Node(
    b.id++,        //id
    Node::Root,    //type
    Piece::ERROR,  //Start
    {},            //parent
    {},            // leaves
    0              //leafPosInParent
    ));
  b.currentNode = b.rootNode.get();

  driver(&b);

  return b.rootNode;
}
