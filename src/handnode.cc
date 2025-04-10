#include "handnode.h"

#include <algorithm>

std::ostream& operator<<(std::ostream& os, const mahjong::Node& node) {
  os << "{ id: " << node.id << ", type:" << mahjong::Node::TypeToStr(node.type);
  os << ", start:" << node.start.toStr();
  os << ", parent: "
     << (node.parent != nullptr ? std::to_string(node.parent->id)
                                : "No Parent");
  os << ", leaves: [ ";
  for (const auto& leaf : node.leaves) {
    os << "id: " << leaf->id << ", ";
  }
  os << " ], "
     << "leafPosInParent: " << node.leafPosInParent << " },";
  os << '\n';
  return os;
}

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
  for (Node* leaf : leaves) {
    delete leaf;
  }
}

Node::ConstIterator& Node::ConstIterator::operator++() {
  if (!root_->leaves.empty()) {
    root_ = root_->leaves.front();
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
      traveler->parent->leaves[leaf_pos_next] != root_) {
    root_ = traveler->parent->leaves[leaf_pos_next];
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

std::string Node::TypeToStr(uint8_t nodetype) {
  switch (nodetype) {
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

std::ostream& Node::DumpAsTGF(std::ostream& os) const {
  std::vector<std::string> nodes;
  std::vector<std::string> connections;
  for (const auto& node : *this) {
    nodes.push_back(std::to_string(node.id) + " Piece: " +
                    (node.type != kRoot ? node.start.toStr() : "Root") +
                    " Type: " + TypeToStr(node.type));
    for (const auto& leaf : node.leaves) {
      connections.push_back(std::to_string(node.id) + " " +
                            std::to_string(leaf->id));
    }
  }
  for (auto const& node : nodes) {
    os << node << '\n';
  }
  os << "#" << '\n';
  for (auto const& connection : connections) {
    os << connection << '\n';
  }
  return os;
}

std::ostream& Node::DumpAsDot(std::ostream& os) const {
  std::vector<std::string> nodes;
  std::vector<std::string> connections;
  for (const auto& node : *this) {
    nodes.push_back(std::to_string(node.id) + " [label=\"" +
                    TypeToStr(node.type) + ": " +
                    (node.type != kRoot ? node.start.toStr() : "Root") + "\"" +
                    ",shape=" + NodeTypeToShapeStr(node.type) +
                    ",color=" + NodeTypeToColorStr(node.type) + "];");
    for (const auto& leaf : node.leaves) {
      connections.push_back(std::to_string(node.id) + " -> " +
                            std::to_string(leaf->id) + ";");
    }
  }
  os << "digraph {" << '\n';
  for (auto const& node : nodes) {
    os << "  " << node << '\n';
  }
  os << '\n';
  for (auto const& connection : connections) {
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
      nodeloc.push_back(nodeloc.back()->leaves[0]);
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
        nodeloc.push_back(nodeloc.back()->leaves[next]);
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
