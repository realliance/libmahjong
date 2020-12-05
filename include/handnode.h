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

namespace Mahjong {

class Node : public std::enable_shared_from_this<Node> {
 public:
  enum Type {
    Error,
    ChiSet,
    PonSet,
    Pair,
    Single,
    Root
  };

  int id;
  Type type;
  Piece start;
  Node* parent;
  std::vector<Node*> leaves;
  size_t leafPosInParent;

  auto operator!=(const Node& n) const -> bool;

  Node(int id, Type type, Piece start, Node* parent, std::vector<Node*> leaves, size_t leafPosInParent) : id(id), type(type), start(start), parent(parent), leaves(std::move(leaves)), leafPosInParent(leafPosInParent) {}

  ~Node();
  class const_iterator {
    const Node* root;
    bool end;
    friend Node;
    explicit const_iterator(const Node* root, bool end) : root(root), end(end) {}

   public:
    auto operator++() -> const_iterator&;
    auto operator*() const -> const Node&;
    auto operator!=(const const_iterator& other) const -> bool;
    // iterator traits
    using difference_type = std::ptrdiff_t;
    using pointer = const Node*;
    using reference = const Node&;
    using iterator_category = std::forward_iterator_tag;
  };
  class iterator {
    const_iterator itr;
    friend Node;
    explicit iterator(Node* root, bool end) : itr(root, end) {}

   public:
    auto operator++() -> iterator&;
    auto operator*() const -> Node&;
    auto operator!=(const iterator& other) const -> bool;
    // iterator traits
    using difference_type = std::ptrdiff_t;
    using pointer = Node*;
    using reference = const Node&;
    using iterator_category = std::forward_iterator_tag;
  };
  static auto TypeToStr(uint8_t nodetype) -> std::string;
  auto DumpAsTGF(std::ostream& os) const -> std::ostream&;
  auto DumpAsDot(std::ostream& os) const -> std::ostream&;
  [[nodiscard]] static auto AsBranchVectors(const Node* root) -> std::vector<std::vector<const Node*>>;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto begin() const -> const_iterator;
  [[nodiscard]] auto end() const -> const_iterator;
  [[nodiscard]] auto begin() -> iterator;
  [[nodiscard]] auto end() -> iterator;
};

}  // namespace Mahjong

auto operator<<(std::ostream& os, const Mahjong::Node& node) -> std::ostream&;
