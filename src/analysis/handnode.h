#pragma once
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "types/piecetype.h"
#include "types/sets.h"

namespace mahjong {

class Node : public std::enable_shared_from_this<Node> {
 public:
  bool operator!=(const Node& n) const;

  explicit Node(int id) : id_(id) {}

  Node(int id, SetType type, Piece start, Node* parent)
      : id_(id), type_(type), start_(start), parent_(parent) {}

  std::string typeToStr() const;

  Node* addLeaf(Piece start, SetType type, int id) {
    leaves_.push_back(std::make_unique<Node>(
        /*id=*/id,
        /*type=*/type,
        /*start=*/start,
        /*parent=*/this));
    return leaves_.back().get();
  }

  [[nodiscard]] static std::vector<std::vector<const Node*>> AsBranchVectors(
      const Node* root);

  std::ostream& DumpAsTGF(std::ostream& os) const;
  std::ostream& DumpAsDot(std::ostream& os) const;
  [[nodiscard]] bool IsComplete() const;

  // Iterators
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
  [[nodiscard]] ConstIterator begin() const;
  [[nodiscard]] ConstIterator end() const;
  [[nodiscard]] Iterator begin();
  [[nodiscard]] Iterator end();

  size_t leafPosInParent() const {
    if (!parent_) {
      return 0;
    }
    auto leaf_it = std::find_if(
        parent_->leaves_.begin(), parent_->leaves_.end(),
        [id = this->id_](const auto& leaf) { return leaf->id_ == id; });
    return std::distance(parent_->leaves_.begin(), leaf_it);
  }

  int id() const { return id_; }
  SetType type() const { return type_; }
  Piece start() const { return start_; }
  Node* parent() { return parent_; }
  Node const* parent() const { return parent_; }
  const std::vector<std::unique_ptr<Node>>& leaves() const { return leaves_; }
  std::vector<std::unique_ptr<Node>>& leaves() { return leaves_; }

 private:
  int id_;
  SetType type_;
  Piece start_;
  Node* parent_ = nullptr;  // not owned.
  std::vector<std::unique_ptr<Node>> leaves_;
};

}  // namespace mahjong
