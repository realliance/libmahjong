#include "analysis/handnode.h"

#include <algorithm>
#include <cstddef>
#include <format>
#include <iostream>
#include <string>
#include <vector>

#include "types/piecetype.h"
#include "types/sets.h"
#include "types/typeprinter.h"

namespace mahjong {

namespace {
std::string NodeTypeToColorStr(SetType nodetype) {
  switch (nodetype) {
    case SetType::kChi:
      return "purple";
    case SetType::kPon:
      return "yellow";
    case SetType::kPair:
      return "green";
    case SetType::kSingle:
      return "blue";
    default:
      return "red";
  }
}

std::string NodeTypeToShapeStr(SetType nodetype) {
  switch (nodetype) {
    case SetType::kChi:
      return "house";
    case SetType::kPon:
      return "septagon";
    case SetType::kPair:
      return "oval";
    case SetType::kSingle:
      return "box";
    default:
      return "Mdiamond";
  }
}
}  // namespace

std::string Node::typeToStr() const {
  switch (type_) {
    case kChi:
      return "Chi";
    case kPon:
      return "Pon";
    case kPair:
      return "Pair";
    case kSingle:
      return "Single";
    default:
      return "Error";
  }
}

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
  return id_ == n.id_ && type_ == n.type_ && start_ == n.start_ &&
         parent_ == n.parent_ && leaves_ == n.leaves_;
}

Node::ConstIterator& Node::ConstIterator::operator++() {
  if (!root_->leaves_.empty()) {
    root_ = root_->leaves_.front().get();
    return *this;
  }
  if (root_->parent_ == nullptr) {
    end_ = true;
    return *this;
  }
  const Node* traveler = root_;
  size_t leaf_pos_next = traveler->leafPosInParent() + 1;
  while ((traveler->parent_ != nullptr) &&
         traveler->parent_->leaves_.size() <= leaf_pos_next) {
    traveler = traveler->parent_;
    leaf_pos_next = traveler->leafPosInParent() + 1;
  }
  if (traveler->parent_ == nullptr) {
    end_ = true;
    return *this;
  }

  if (traveler->parent_->leaves_.size() > leaf_pos_next &&
      traveler->parent_->leaves_[leaf_pos_next].get() != root_) {
    root_ = traveler->parent_->leaves_[leaf_pos_next].get();
    return *this;
  }
  std::cerr << "FORWARD TRAVERSAL FAILED: Set to end." << '\n';
  std::cerr << "ROOT: " << root_ << '\n';
  if (traveler->parent_ != nullptr) {
    std::cerr << "PARENT: " << traveler->parent_ << '\n';
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
    const bool is_root = node.parent_ == nullptr;
    const std::string piece = is_root ? "Root" : node.start().toStr();
    const std::string type = is_root ? "RootNode" : node.typeToStr();
    nodes.push_back(
        std::format("{} Piece: {} Type: {}", node.id_, piece, type));
    for (const auto& leaf : node.leaves_) {
      connections.push_back(std::format("{} {}", node.id_, leaf->id_));
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
    const bool is_root = node.parent_ == nullptr;
    const std::string piece = is_root ? "Root" : node.start().toStr();
    const std::string type = is_root ? "RootNode" : node.typeToStr();
    const std::string shape =
        is_root ? "underline" : NodeTypeToShapeStr(node.type_);
    const std::string color =
        is_root ? "black" : NodeTypeToColorStr(node.type_);
    nodes.push_back(std::format("{} [label=\"{}: {}\",shape={},color={}];",
                                node.id_, piece, type, shape, color));
    for (const auto& leaf : node.leaves_) {
      connections.push_back(std::format("{} -> {};", node.id_, leaf->id_));
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
  if (!root) {
    std::cerr << "ERROR: AsBranchVectors called with null root\n";
    return {};
  }

  std::vector<std::vector<const Node*>> branches;
  std::vector<const Node*> nodeloc;
  nodeloc.push_back(root);

  while (!nodeloc.empty()) {
    if (nodeloc.empty()) {
      std::cerr << "ERROR: AsBranchVectors nodeloc became empty unexpectedly\n";
      break;
    }

    const Node* current = nodeloc.back();
    if (!current) {
      std::cerr << "ERROR: AsBranchVectors null node in nodeloc\n";
      nodeloc.pop_back();
      continue;
    }

    if (!current->leaves_.empty()) {
      nodeloc.push_back(current->leaves_[0].get());
    } else {
      // Skip the root node when adding to branches list.
      branches.emplace_back(nodeloc.begin() + 1, nodeloc.end());
      size_t next = current->leafPosInParent() + 1;

      while ((current->parent_ != nullptr) &&
             current->parent_->leaves_.size() <= next) {
        nodeloc.pop_back();
        if (nodeloc.empty()) {
          break;
        }
        current = nodeloc.back();
        next = current->leafPosInParent() + 1;
      }

      if (nodeloc.empty() || current->parent_ == nullptr) {
        nodeloc.pop_back();
      } else {
        nodeloc.pop_back();
        if (!nodeloc.empty() && next < nodeloc.back()->leaves_.size()) {
          nodeloc.push_back(nodeloc.back()->leaves_[next].get());
        } else {
          std::cerr << "ERROR: AsBranchVectors next index " << next
                    << " out of bounds\n";
          break;
        }
      }
    }
  }
  return branches;
}

bool Node::IsComplete() const {
  auto branches = AsBranchVectors(this);
  return std::ranges::any_of(branches, [](auto branch) {
    int pair_count = 0;
    return std::none_of(branch.begin(), branch.end(),
                        [&pair_count](auto node) {
                          if (node->type_ == SetType::kPair) {
                            pair_count += 1;
                          }
                          return node->type_ == SetType::kSingle;
                        }) &&
           (pair_count == 1 || pair_count == 7);
  });
}
}  // namespace mahjong
