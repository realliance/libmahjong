#include "handnode.h"

#include <algorithm>

auto operator<<(std::ostream& os, const mahjong::Node& node) -> std::ostream& {
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
auto NodeTypeToColorStr(uint8_t nodetype) -> std::string {
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

auto NodeTypeToShapeStr(uint8_t nodetype) -> std::string {
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

auto Node::begin() const -> ConstIterator {
  return ConstIterator(this, /*end=*/false);
}

auto Node::end() const -> ConstIterator {
  return ConstIterator(this, /*end=*/true);
}

auto Node::begin() -> Iterator {
  return Iterator(this, /*end=*/false);
}

auto Node::end() -> Iterator {
  return Iterator(this, /*end=*/true);
}

auto Node::operator!=(const Node& n) const -> bool {
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

auto Node::ConstIterator::operator++() -> ConstIterator& {
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

auto Node::Iterator::operator++() -> Iterator& {
  itr_.operator++();
  return *this;
}

auto Node::Iterator::operator*() const -> Node& {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
  return const_cast<Node&>(*itr_.root_);
}

auto Node::ConstIterator::operator*() const -> const Node& {
  return *root_;
}

auto Node::Iterator::operator!=(const Iterator& other) const -> bool {
  return itr_ != other.itr_;
}

auto Node::ConstIterator::operator!=(const Node::ConstIterator& other) const
    -> bool {
  return end_ != other.end_;
}

auto Node::TypeToStr(uint8_t nodetype) -> std::string {
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

auto Node::DumpAsTGF(std::ostream& os) const -> std::ostream& {
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

auto Node::DumpAsDot(std::ostream& os) const -> std::ostream& {
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

auto Node::AsBranchVectors(const Node* root)
    -> std::vector<std::vector<const Node*>> {
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

auto Node::IsComplete() const -> bool {
  auto branches = AsBranchVectors(this);
  return std::ranges::any_of(branches, [](auto branch) {
    return std::none_of(branch.begin(), branch.end(),
                        [](auto node) { return node->type == Node::kSingle; });
  });
}
}  // namespace mahjong
