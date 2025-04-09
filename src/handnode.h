#pragma once
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "piecetype.h"

namespace mahjong {

class Node : public std::enable_shared_from_this<Node> {
 public:
  enum Type { kError, kChiSet, kPonSet, kPair, kSingle, kRoot };

  int id;
  Type type;
  Piece start;
  Node* parent;
  std::vector<Node*> leaves;
  size_t leafPosInParent;

  auto operator!=(const Node& n) const -> bool;

  Node(int id, Type type, Piece start, Node* parent, std::vector<Node*> leaves,
       size_t leafPosInParent)
      : id(id),
        type(type),
        start(start),
        parent(parent),
        leaves(std::move(leaves)),
        leafPosInParent(leafPosInParent) {}

  ~Node();
  class ConstIterator {
    const Node* root_;
    bool end_;
    friend Node;
    explicit ConstIterator(const Node* root, bool end)
        : root_(root), end_(end) {}

   public:
    auto operator++() -> ConstIterator&;
    auto operator*() const -> const Node&;
    auto operator!=(const ConstIterator& other) const -> bool;
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
    auto operator++() -> Iterator&;
    auto operator*() const -> Node&;
    auto operator!=(const Iterator& other) const -> bool;
    // iterator traits
    using difference_type = std::ptrdiff_t;
    using pointer = Node*;
    using iterator_category = std::forward_iterator_tag;
  };
  static auto TypeToStr(uint8_t nodetype) -> std::string;
  auto DumpAsTGF(std::ostream& os) const -> std::ostream&;
  auto DumpAsDot(std::ostream& os) const -> std::ostream&;
  [[nodiscard]] static auto AsBranchVectors(const Node* root)
      -> std::vector<std::vector<const Node*>>;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto begin() const -> ConstIterator;
  [[nodiscard]] auto end() const -> ConstIterator;
  [[nodiscard]] auto begin() -> Iterator;
  [[nodiscard]] auto end() -> Iterator;
};

}  // namespace mahjong

auto operator<<(std::ostream& os, const mahjong::Node& node) -> std::ostream&;
