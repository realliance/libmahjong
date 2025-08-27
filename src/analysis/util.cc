#include "analysis/util.h"

#include <algorithm>
#include <cstdlib>
#include <map>
#include <vector>

#include "analysis/analysis.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"

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

std::vector<Piece> completeSet(const Piece a, const Piece b) {
  if (a.getSuit() != b.getSuit()) {
    return {};
  }
  const int piece_diff = static_cast<int>(a.getPieceNum()) - b.getPieceNum();
  if (std::abs(piece_diff) > 2) {
    return {};
  }
  if (std::abs(piece_diff) != 1) {
    // piece_diff: -2 / 2
    // piece_diff >> 1: -1 / 1
    // b + piece_diff >> 1: c
    // a - b == -2: b (c) a
    // a - b ==  2: a (c) b
    return {b + (piece_diff >> 1)};
  }
  std::vector<Piece> waits;
  if (a + piece_diff != kError) {
    waits.push_back(a + piece_diff);
  }
  if (b - piece_diff != kError) {
    waits.push_back(b - piece_diff);
  }
  return waits;
}

std::vector<Piece> getWaits(const Hand& hand) {
  // 13, 10, 7, 4, 1 are possible piece counts for non-melded tiles
  if (hand.live.empty() || hand.live.size() % 3 != 1) {
    return {};
  }
  constexpr int kMaxSingles = 2;
  std::vector<Piece> waits;
  for (const Branch& branch : AnalyzeHand(hand)) {
    if (branch.singles.size() > kMaxSingles) {
      continue;
    }
    if (branch.singles.size() == 1 &&
        (branch.pairs.empty() || branch.pairs.size() == 6)) {
      waits.emplace_back(branch.singles.front());
    } else if (branch.singles.size() == 2) {
      waits.append_range(completeSet(branch.singles[0], branch.singles[1]));
    }
  }
  return waits;
}

std::vector<Piece> getWaits(const Hand& hand, const Piece& piece) {
  Hand new_hand = hand;
  new_hand.live.erase(std::ranges::find(new_hand.live, piece));
  return getWaits(new_hand);
}

std::map<Piece, std::vector<Piece>> getPossibleWaits(const Hand& hand) {
  // 14, 11, 8, 5, 2 are the possible piece counts for non-melded tiles.
  if (hand.live.empty() || hand.live.size() % 3 != 2) {
    return {};
  }
  constexpr int kMaxSingles = 3;
  std::map<Piece, std::vector<Piece>> waits;
  for (const Branch& branch : AnalyzeHand(hand)) {
    if (branch.singles.size() > kMaxSingles) {
      continue;
    }
    if (branch.singles.size() == 2 &&
        (branch.pairs.empty() || branch.pairs.size() == 6)) {
      waits[branch.singles[0]].emplace_back(branch.singles[1]);
      waits[branch.singles[1]].emplace_back(branch.singles[0]);
    } else if (branch.singles.size() == 3) {
      waits[branch.singles[0]].append_range(
          completeSet(branch.singles[1], branch.singles[2]));
      waits[branch.singles[1]].append_range(
          completeSet(branch.singles[2], branch.singles[0]));
      waits[branch.singles[2]].append_range(
          completeSet(branch.singles[0], branch.singles[1]));
    }
  }
  return waits;
}

bool CheckBranch(const Branch& branch, const SetCheckFunc& func) {
  return std::ranges::all_of(
             branch.chis,
             [&func](Piece p) { return func(SetType::kChi, p); }) &&
         std::ranges::all_of(
             branch.pons,
             [&func](Piece p) { return func(SetType::kPon, p); }) &&
         std::ranges::all_of(
             branch.kans,
             [&func](Piece p) { return func(SetType::kKan, p); }) &&
         std::ranges::all_of(
             branch.pairs,
             [&func](Piece p) { return func(SetType::kSingle, p); }) &&
         std::ranges::all_of(branch.singles, [&func](Piece p) {
           return func(SetType::kPair, p);
         });
}

}  // namespace mahjong
