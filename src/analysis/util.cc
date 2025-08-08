#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>
#include "analysis/analysis.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong {
namespace {
const std::vector<Piece> kPieceSet{
    kOneBamboo,    kTwoBamboo,      kThreeBamboo,    kFourBamboo,
    kFiveBamboo,   kSixBamboo,      kSevenBamboo,    kEightBamboo,
    kNineBamboo,   kOnePin,         kTwoPin,         kThreePin,
    kFourPin,      kFivePin,        kSixPin,         kSevenPin,
    kEightPin,     kNinePin,        kOneCharacter,   kNineCharacter,
    kTwoCharacter, kThreeCharacter, kFourCharacter,  kFiveCharacter,
    kSixCharacter, kSevenCharacter, kEightCharacter, kWhiteDragon,
    kGreenDragon,  kRedDragon,      kEastWind,       kSouthWind,
    kNorthWind,    kWestWind,
};
}  // namespace

std::vector<Piece> getRiichiDiscard(std::vector<Piece> hand) {
  if (hand.empty()) {
    return {};
  }
  std::array<int8_t, Piece::kPiecesize> counts = {};
  std::array<bool, Piece::kPiecesize> removedbefore = {};
  std::vector<Piece> remove_me;
  for (const auto& p : hand) {
    counts.at(p.toUint8_t())++;
  }
  for (int i = 0; i < 9; i++) {
    const Piece removed = hand.front();
    hand.erase(hand.begin());
    if (removedbefore.at(removed.toUint8_t())) {
      hand.push_back(removed);
      continue;
    }
    removedbefore.at(removed.toUint8_t()) = true;
    for (const auto& p : kPieceSet) {
      if (counts.at(p.toUint8_t()) == 4 || p == removed) {
        continue;
      }
      hand.push_back(p);
      auto root = breakdownHand(hand);
      if (root->IsComplete()) {
        remove_me.push_back(removed);
      }

      hand.pop_back();
    }
    hand.push_back(removed);
  }
  return remove_me;
}

// TODO(#20): This is an extremely inefficient algorithm but it's probably good enough for
// the frequency it needs to be ran
// will revisit if necessary
// assumption is 14 piece hand
std::vector<Piece> isInTenpai13Pieces(std::vector<Piece> hand, bool allWaits) {
  const int min_singles = countSingles(hand);
  // These numbers were found by looking at a lot of handtrees and their single count
  if (min_singles > 5 || min_singles == 3 || min_singles == 0) {
    return {};
  }
  std::array<int8_t, Piece::kPiecesize> counts = {};
  std::vector<Piece> waits;
  for (const auto& p : hand) {
    counts.at(p.toUint8_t())++;
  }
  for (const auto& p : kPieceSet) {
    if (counts.at(p.toUint8_t()) == 4) {
      continue;
    }
    hand.push_back(p);
    auto root = breakdownHand(hand);
    if (root->IsComplete()) {
      waits.push_back(p);
      if (!allWaits) {
        return waits;
      }
    }

    hand.pop_back();
  }
  return waits;
}

const int kPiecesinahand = 14;

std::vector<Piece> isInTenpai(std::vector<Piece> hand, bool allWaits) {
  if (hand.empty()) {
    return {};
  }
  const int min_singles = countSingles(hand);
  // These numbers are the same as above except one more piece means 1 higher on the single count
  if (min_singles > 6 || min_singles == 1 || min_singles == 4 ||
      min_singles == 0) {
    return {};
  }
  std::array<bool, Piece::kPiecesize> removedbefore = {};
  std::vector<Piece> waits;
  for (int i = 0; i < kPiecesinahand; i++) {
    const Piece removed = hand.front();
    hand.erase(hand.begin());
    if (removedbefore.at(removed.toUint8_t())) {
      hand.push_back(removed);
      continue;
    }
    removedbefore.at(removed.toUint8_t()) = true;
    std::vector<Piece> tempwaits = isInTenpai13Pieces(hand, allWaits);
    if (!tempwaits.empty()) {
      if (!allWaits) {
        return tempwaits;
      }
      waits.insert(waits.begin(), tempwaits.begin(), tempwaits.end());
    }
    hand.push_back(removed);
  }
  return waits;
}

int countSingles(const std::vector<Piece>& hand) {
  auto root = breakdownHand(hand);
  int min_singles = 15;
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    int singles = 0;
    for (const auto& node : branch) {
      if (node->type() == Node::kSingle) {
        singles++;
      }
    }
    min_singles = std::min(singles, min_singles);
  }
  return min_singles;
}

int countPiece(const GameState& state, int player, Piece p) {
  int count = 0;
  for (const auto& piece : state.hands.at(player).live) {
    if (piece == p) {
      count++;
    }
  }
  return count;
}

}  // namespace mahjong
