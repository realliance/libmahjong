#pragma once
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "piecetype.h"

namespace mahjong {

class Node : public std::enable_shared_from_this<Node> {
 public:
  enum Type { kError, kChiSet, kPonSet, kPair, kSingle, kRoot };

  int id;
  Type type;
  Piece start;
  Node* parent = nullptr;  // not owned.
  std::vector<std::unique_ptr<Node>> leaves;
  size_t leafPosInParent;

  bool operator!=(const Node& n) const;

  Node(int id, Type type, Piece start, Node* parent = nullptr,
       size_t leafPosInParent = 0)
      : id(id),
        type(type),
        start(start),
        parent(parent),
        leafPosInParent(leafPosInParent) {}

  ~Node();
  class ConstIterator {
    const Node* root_;
    bool end_;
    friend Node;
    explicit ConstIterator(const Node* root, bool end)
        : root_(root), end_(end) {}

   public:
    ConstIterator& operator++();
    const Node& operator*() const;
    bool operator!=(const ConstIterator& other) const;
    // iterator traits
    using difference_type = std::ptrdiff_t;
    using pointer = const Node*;
    using iterator_category = std::forward_iterator_tag;
  };
  class Iterator {
    ConstIterator itr_;
    friend Node;
    explicit Iterator(Node* root, bool end) : itr_(root, end) {}

   public:
    Iterator& operator++();
    Node& operator*() const;
    bool operator!=(const Iterator& other) const;
    // iterator traits
    using difference_type = std::ptrdiff_t;
    using pointer = Node*;
    using iterator_category = std::forward_iterator_tag;
  };

  std::string typeToStr() const {
    switch (type) {
      case kChiSet:
        return "Chi";
      case kPonSet:
        return "Pon";
      case kPair:
        return "Pair";
      case kSingle:
        return "Single";
      case kRoot:
        return "RootNode";
      default:
        return "Invalid Type";
    }
  }

  std::ostream& DumpAsTGF(std::ostream& os) const;
  std::ostream& DumpAsDot(std::ostream& os) const;
  [[nodiscard]] static std::vector<std::vector<const Node*>> AsBranchVectors(
      const Node* root);
  [[nodiscard]] bool IsComplete() const;
  [[nodiscard]] ConstIterator begin() const;
  [[nodiscard]] ConstIterator end() const;
  [[nodiscard]] Iterator begin();
  [[nodiscard]] Iterator end();
};

}  // namespace mahjong
