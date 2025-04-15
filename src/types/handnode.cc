#include "handnode.h"

#include <algorithm>
#include <cstdint>

#include "typeprinter.h"
#include "types/piecetype.h"

namespace mahjong {

namespace {
std::string NodeTypeToColorStr(uint8_t nodetype) {
  switch (nodetype) {
    case Node::kChiSet:
      return "purple";
    case Node::kPonSet:
      return "yellow";
    case Node::kPair:
      return "green";
    case Node::kSingle:
      return "blue";
    case Node::kRoot:
      return "black";
    default:
      return "red";
  }
}

std::string NodeTypeToShapeStr(uint8_t nodetype) {
  switch (nodetype) {
    case Node::kChiSet:
      return "house";
    case Node::kPonSet:
      return "septagon";
    case Node::kPair:
      return "oval";
    case Node::kSingle:
      return "box";
    case Node::kRoot:
      return "underline";
    default:
      return "Mdiamond";
  }
}
}  // namespace

Node::ConstIterator Node::begin() const {
  return ConstIterator(this, /*end=*/false);
}

Node::ConstIterator Node::end() const {
  return ConstIterator(this, /*end=*/true);
}

Node::Iterator Node::begin() {
  return Iterator(this, /*end=*/false);
}

Node::Iterator Node::end() {
  return Iterator(this, /*end=*/true);
}

bool Node::operator!=(const Node& n) const {
  return id == n.id && type == n.type && start == n.start &&
         parent == n.parent && leaves == n.leaves &&
         leafPosInParent == n.leafPosInParent;
}

Node::~Node() {
  if (parent != nullptr) {
    parent->leaves.erase(parent->leaves.begin() + leafPosInParent);
    for (size_t i = 0; i < parent->leaves.size(); i++) {
      parent->leaves.at(i)->leafPosInParent = i;
    }
  }
}

Node::ConstIterator& Node::ConstIterator::operator++() {
  if (!root_->leaves.empty()) {
    root_ = root_->leaves.front().get();
    return *this;
  }
  if (root_->parent == nullptr) {
    end_ = true;
    return *this;
  }
  const Node* traveler = root_;
  size_t leaf_pos_next = traveler->leafPosInParent + 1;
  while ((traveler->parent != nullptr) &&
         traveler->parent->leaves.size() <= leaf_pos_next) {
    traveler = traveler->parent;
    leaf_pos_next = traveler->leafPosInParent + 1;
  }
  if (traveler->parent == nullptr) {
    end_ = true;
    return *this;
  }

  if (traveler->parent->leaves.size() > leaf_pos_next &&
      traveler->parent->leaves[leaf_pos_next].get() != root_) {
    root_ = traveler->parent->leaves[leaf_pos_next].get();
    return *this;
  }
  std::cerr << "FORWARD TRAVERSAL FAILED: Set to end." << '\n';
  std::cerr << "ROOT: " << root_ << '\n';
  if (traveler->parent != nullptr) {
    std::cerr << "PARENT: " << traveler->parent << '\n';
  }
  std::cerr << "TRAVELER: " << *traveler << '\n';
  end_ = true;
  return *this;
}

Node::Iterator& Node::Iterator::operator++() {
  itr_.operator++();
  return *this;
}

Node& Node::Iterator::operator*() const {
  return const_cast<Node&>(*itr_.root_);
}

const Node& Node::ConstIterator::operator*() const {
  return *root_;
}

bool Node::Iterator::operator!=(const Iterator& other) const {
  return itr_ != other.itr_;
}

bool Node::ConstIterator::operator!=(const Node::ConstIterator& other) const {
  return end_ != other.end_;
}

std::ostream& Node::DumpAsTGF(std::ostream& os) const {
  std::vector<std::string> nodes;
  std::vector<std::string> connections;
  for (const auto& node : *this) {
    nodes.push_back(std::to_string(node.id) + " Piece: " +
                    (node.type != kRoot ? node.start.toStr() : "Root") +
                    " Type: " + node.typeToStr());
    for (const auto& leaf : node.leaves) {
      connections.push_back(std::to_string(node.id) + " " +
                            std::to_string(leaf->id));
    }
  }
  for (const auto& node : nodes) {
    os << node << '\n';
  }
  os << "#" << '\n';
  for (const auto& connection : connections) {
    os << connection << '\n';
  }
  return os;
}

std::ostream& Node::DumpAsDot(std::ostream& os) const {
  std::vector<std::string> nodes;
  std::vector<std::string> connections;
  for (const auto& node : *this) {
    nodes.push_back(std::to_string(node.id) + " [label=\"" + node.typeToStr() +
                    ": " + (node.type != kRoot ? node.start.toStr() : "Root") +
                    "\"" + ",shape=" + NodeTypeToShapeStr(node.type) +
                    ",color=" + NodeTypeToColorStr(node.type) + "];");
    for (const auto& leaf : node.leaves) {
      connections.push_back(std::to_string(node.id) + " -> " +
                            std::to_string(leaf->id) + ";");
    }
  }
  os << "digraph {" << '\n';
  for (const auto& node : nodes) {
    os << "  " << node << '\n';
  }
  os << '\n';
  for (const auto& connection : connections) {
    os << "  " << connection << '\n';
  }
  os << "}" << '\n';
  return os;
}

std::vector<std::vector<const Node*>> Node::AsBranchVectors(const Node* root) {
  std::vector<std::vector<const Node*>> branches;
  std::vector<const Node*> nodeloc;
  nodeloc.push_back(root);
  while (!nodeloc.empty()) {
    if (!nodeloc.back()->leaves.empty()) {
      nodeloc.push_back(nodeloc.back()->leaves[0].get());
    } else {
      branches.push_back(nodeloc);
      size_t next = nodeloc.back()->leafPosInParent + 1;
      while ((nodeloc.back()->parent != nullptr) &&
             nodeloc.back()->parent->leaves.size() <= next) {
        nodeloc.pop_back();
        next = nodeloc.back()->leafPosInParent + 1;
      }
      if (nodeloc.back()->parent == nullptr) {
        nodeloc.pop_back();
      } else {
        nodeloc.pop_back();
        nodeloc.push_back(nodeloc.back()->leaves[next].get());
      }
    }
  }
  return branches;
}

bool Node::IsComplete() const {
  auto branches = AsBranchVectors(this);
  return std::ranges::any_of(branches, [](auto branch) {
    return std::none_of(branch.begin(), branch.end(),
                        [](auto node) { return node->type == Node::kSingle; });
  });
}
}  // namespace mahjong
