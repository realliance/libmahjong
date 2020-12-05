#include "handnode.h"

#include <algorithm>

using Mahjong::Node;

auto Node::begin() const -> const_iterator {
  return const_iterator(this, /*end=*/false);
}

auto Node::end() const -> const_iterator {
  return const_iterator(this, /*end=*/true);
}

auto Node::begin() -> iterator {
  return iterator(this, /*end=*/false);
}

auto Node::end() -> iterator {
  return iterator(this, /*end=*/true);
}

auto Node::operator!=(const Node& n) const -> bool {
  return id == n.id &&
         type == n.type &&
         start == n.start &&
         parent == n.parent &&
         leaves == n.leaves &&
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
    leaf->parent = nullptr;
    delete leaf;
  }
}

auto Node::const_iterator::operator++() -> const_iterator& {
  if (!root->leaves.empty()) {
    root = root->leaves.front();
    return *this;
  }
  if (root->parent == nullptr) {
    end = true;
    return *this;
  }
  const Node* traveler = root;
  size_t leafPosNext = traveler->leafPosInParent + 1;
  while ((traveler->parent != nullptr) && traveler->parent->leaves.size() <= leafPosNext) {
    traveler = traveler->parent;
    leafPosNext = traveler->leafPosInParent + 1;
  }
  if (traveler->parent == nullptr) {
    end = true;
    return *this;
  }

  if (traveler->parent->leaves.size() > leafPosNext && traveler->parent->leaves[leafPosNext] != root) {
    root = traveler->parent->leaves[leafPosNext];
    return *this;
  }
  std::cerr << "FORWARD TRAVERSAL FAILED: Set to end." << std::endl;
  std::cerr << "ROOT: " << root << std::endl;
  if (traveler->parent != nullptr) {
    std::cerr << "PARENT: " << traveler->parent << std::endl;
  }
  std::cerr << "TRAVELER: " << *traveler << std::endl;
  end = true;
  return *this;
}

auto Node::iterator::operator++() -> iterator& {
  itr.operator++();
  return *this;
}

auto Node::iterator::operator*() const -> Node& {
  //NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
  return const_cast<Node&>(*itr.root);
}

auto Node::const_iterator::operator*() const -> const Node& {
  return *root;
}

auto Node::iterator::operator!=(const iterator& other) const -> bool {
  return itr != other.itr;
}

auto Node::const_iterator::operator!=(const Node::const_iterator& other) const -> bool {
  return end != other.end;
}

auto Node::TypeToStr(uint8_t nodetype) -> std::string {
  switch (nodetype) {
    case ChiSet:
      return "Chi";
    case PonSet:
      return "Pon";
    case Pair:
      return "Pair";
    case Single:
      return "Single";
    case Root:
      return "RootNode";
    default:
      return "Invalid Type";
  }
}

auto Node::DumpAsTGF(std::ostream& os) const -> std::ostream& {
  std::vector<std::string> nodes;
  std::vector<std::string> connections;
  for (const auto& node : *this) {
    nodes.push_back(std::to_string(node.id) + " Piece: " + (node.type != Root ? node.start.toStr() : "Root") + " Type: " + TypeToStr(node.type));
    for (const auto& leaf : node.leaves) {
      connections.push_back(std::to_string(node.id) + " " + std::to_string(leaf->id));
    }
  }
  for (auto const& node : nodes) {
    os << node << std::endl;
  }
  os << "#" << std::endl;
  for (auto const& connection : connections) {
    os << connection << std::endl;
  }
  return os;
}

auto NodeTypeToColorStr(uint8_t nodetype) -> std::string {
  switch (nodetype) {
    case Node::ChiSet:
      return "purple";
    case Node::PonSet:
      return "yellow";
    case Node::Pair:
      return "green";
    case Node::Single:
      return "blue";
    case Node::Root:
      return "black";
    default:
      return "red";
  }
}

auto NodeTypeToShapeStr(uint8_t nodetype) -> std::string {
  switch (nodetype) {
    case Node::ChiSet:
      return "house";
    case Node::PonSet:
      return "septagon";
    case Node::Pair:
      return "oval";
    case Node::Single:
      return "box";
    case Node::Root:
      return "underline";
    default:
      return "Mdiamond";
  }
}

auto Node::DumpAsDot(std::ostream& os) const -> std::ostream& {
  std::vector<std::string> nodes;
  std::vector<std::string> connections;
  for (const auto& node : *this) {
    nodes.push_back(
      std::to_string(node.id) + " [label=\"" + TypeToStr(node.type) + ": " + (node.type != Root ? node.start.toStr() : "Root") + "\"" + ",shape=" + NodeTypeToShapeStr(node.type) + ",color=" + NodeTypeToColorStr(node.type) + "];");
    for (const auto& leaf : node.leaves) {
      connections.push_back(std::to_string(node.id) + " -> " + std::to_string(leaf->id) + ";");
    }
  }
  os << "digraph {" << std::endl;
  for (auto const& node : nodes) {
    os << "  " << node << std::endl;
  }
  os << std::endl;
  for (auto const& connection : connections) {
    os << "  " << connection << std::endl;
  }
  os << "}" << std::endl;
  return os;
}

auto Node::AsBranchVectors(const Node* root) -> std::vector<std::vector<const Node*>> {
  std::vector<std::vector<const Node*>> branches;
  std::vector<const Node*> nodeloc;
  nodeloc.push_back(root);
  while (!nodeloc.empty()) {
    if (!nodeloc.back()->leaves.empty()) {
      nodeloc.push_back(nodeloc.back()->leaves[0]);
    } else {
      branches.push_back(nodeloc);
      size_t next = nodeloc.back()->leafPosInParent + 1;
      while ((nodeloc.back()->parent != nullptr) && nodeloc.back()->parent->leaves.size() <= next) {
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
  return std::any_of(branches.begin(), branches.end(), [](auto branch) {
    return std::none_of(branch.rbegin(), branch.rend(), [](auto node) { return node->type == Node::Single; });
  });
}

auto operator<<(std::ostream& os, const Node& node) -> std::ostream& {
  os << "{ id: " << node.id << ", type:" << Node::TypeToStr(node.type);
  os << ", start:" << node.start.toStr();
  os << ", parent: " << (node.parent != nullptr ? std::to_string(node.parent->id) : "No Parent");
  os << ", leaves: [ ";
  for (const auto& leaf : node.leaves) {
    os << "id: " << leaf->id << ", ";
  }
  os << " ], "
     << "leafPosInParent: " << node.leafPosInParent << " },";
  os << std::endl;
  return os;
}
