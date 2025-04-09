#include <algorithm>
#include <cstdint>
#include <iterator>
#include <map>
#include <memory>
#include <random>
// #include <ext/alloc_traits.h>
#include <array>
#include <utility>
#include <vector>

#include "analysis.h"
#include "handnode.h"
#include "pieces.h"
#include "piecetype.h"

namespace mahjong {
namespace {
const std::vector<std::vector<Piece>> kTriplets = {
    {kOneBamboo, kTwoBamboo, kThreeBamboo},
    {kTwoBamboo, kThreeBamboo, kFourBamboo},
    {kThreeBamboo, kFourBamboo, kFiveBamboo},
    {kFourBamboo, kFiveBamboo, kSixBamboo},
    {kFiveBamboo, kSixBamboo, kSevenBamboo},
    {kSixBamboo, kSevenBamboo, kEightBamboo},
    {kSevenBamboo, kEightBamboo, kNineBamboo},
    {kOneCharacter, kTwoCharacter, kThreeCharacter},
    {kTwoCharacter, kThreeCharacter, kFourCharacter},
    {kThreeCharacter, kFourCharacter, kFiveCharacter},
    {kFourCharacter, kFiveCharacter, kSixCharacter},
    {kFiveCharacter, kSixCharacter, kSevenCharacter},
    {kSixCharacter, kSevenCharacter, kEightCharacter},
    {kSevenCharacter, kEightCharacter, kNineCharacter},
    {kOnePin, kTwoPin, kThreePin},
    {kTwoPin, kThreePin, kFourPin},
    {kThreePin, kFourPin, kFivePin},
    {kFourPin, kFivePin, kSixPin},
    {kFivePin, kSixPin, kSevenPin},
    {kSixPin, kSevenPin, kEightPin},
    {kSevenPin, kEightPin, kNinePin},
    {kOneBamboo, kOneBamboo, kOneBamboo},
    {kTwoBamboo, kTwoBamboo, kTwoBamboo},
    {kThreeBamboo, kThreeBamboo, kThreeBamboo},
    {kFourBamboo, kFourBamboo, kFourBamboo},
    {kFiveBamboo, kFiveBamboo, kFiveBamboo},
    {kSixBamboo, kSixBamboo, kSixBamboo},
    {kSevenBamboo, kSevenBamboo, kSevenBamboo},
    {kEightBamboo, kEightBamboo, kEightBamboo},
    {kNineBamboo, kNineBamboo, kNineBamboo},
    {kOneCharacter, kOneCharacter, kOneCharacter},
    {kTwoCharacter, kTwoCharacter, kTwoCharacter},
    {kThreeCharacter, kThreeCharacter, kThreeCharacter},
    {kFourCharacter, kFourCharacter, kFourCharacter},
    {kFiveCharacter, kFiveCharacter, kFiveCharacter},
    {kSixCharacter, kSixCharacter, kSixCharacter},
    {kSevenCharacter, kSevenCharacter, kSevenCharacter},
    {kEightCharacter, kEightCharacter, kEightCharacter},
    {kNineCharacter, kNineCharacter, kNineCharacter},
    {kOnePin, kOnePin, kOnePin},
    {kTwoPin, kTwoPin, kTwoPin},
    {kThreePin, kThreePin, kThreePin},
    {kFourPin, kFourPin, kFourPin},
    {kFivePin, kFivePin, kFivePin},
    {kSixPin, kSixPin, kSixPin},
    {kSevenPin, kSevenPin, kSevenPin},
    {kEightPin, kEightPin, kEightPin},
    {kNinePin, kNinePin, kNinePin},
    {kWestWind, kWestWind, kWestWind},
    {kEastWind, kEastWind, kEastWind},
    {kSouthWind, kSouthWind, kSouthWind},
    {kNorthWind, kNorthWind, kNorthWind},
    {kRedDragon, kRedDragon, kRedDragon},
    {kWhiteDragon, kWhiteDragon, kWhiteDragon},
    {kGreenDragon, kGreenDragon, kGreenDragon}};

const std::vector<std::vector<Piece>> kPairs = {
    {kOneBamboo, kOneBamboo},
    {kTwoBamboo, kTwoBamboo},
    {kThreeBamboo, kThreeBamboo},
    {kFourBamboo, kFourBamboo},
    {kFiveBamboo, kFiveBamboo},
    {kSixBamboo, kSixBamboo},
    {kSevenBamboo, kSevenBamboo},
    {kEightBamboo, kEightBamboo},
    {kNineBamboo, kNineBamboo},
    {kOnePin, kOnePin},
    {kTwoPin, kTwoPin},
    {kThreePin, kThreePin},
    {kFourPin, kFourPin},
    {kFivePin, kFivePin},
    {kSixPin, kSixPin},
    {kSevenPin, kSevenPin},
    {kEightPin, kEightPin},
    {kNinePin, kNinePin},
    {kOneCharacter, kOneCharacter},
    {kTwoCharacter, kTwoCharacter},
    {kThreeCharacter, kThreeCharacter},
    {kFourCharacter, kFourCharacter},
    {kFiveCharacter, kFiveCharacter},
    {kSixCharacter, kSixCharacter},
    {kSevenCharacter, kSevenCharacter},
    {kEightCharacter, kEightCharacter},
    {kNineCharacter, kNineCharacter},
    {kEastWind, kEastWind},
    {kSouthWind, kSouthWind},
    {kWestWind, kWestWind},
    {kNorthWind, kNorthWind},
    {kRedDragon, kRedDragon},
    {kWhiteDragon, kWhiteDragon},
    {kGreenDragon, kGreenDragon}};

const std::vector<Piece> kPieceSet{
    kOneBamboo,    kTwoBamboo,      kThreeBamboo,    kFourBamboo,
    kFiveBamboo,   kSixBamboo,      kSevenBamboo,    kEightBamboo,
    kNineBamboo,   kOnePin,         kTwoPin,         kThreePin,
    kFourPin,      kFivePin,        kSixPin,         kSevenPin,
    kEightPin,     kNinePin,        kOneCharacter,   kNineCharacter,
    kTwoCharacter, kThreeCharacter, kFourCharacter,  kFiveCharacter,
    kSixCharacter, kSevenCharacter, kEightCharacter, kWhiteDragon,
    kGreenDragon,  kRedDragon,      kEastWind,       kSouthWind,
    kNorthWind,    kWestWind};

auto TestValid(const std::vector<Piece>& hand) -> bool {
  std::map<Piece, bool> pieces;
  for (const auto& piece : hand) {
    if (pieces.contains(piece)) {
      return false;
    }
    pieces[piece] = true;
  }
  return true;
}
}  // namespace

auto GetPossibleStdFormHand() -> std::vector<Piece> {
  std::vector<Piece> living_walls;
  std::vector<Piece> dead_wall;
  for (int i = 0; i < 4; i++) {
    living_walls.insert(living_walls.end(), kPieceSet.begin(), kPieceSet.end());
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(living_walls.begin(), living_walls.end(), g);
  std::move(living_walls.begin(), living_walls.begin() + 14,
            std::back_inserter(dead_wall));

  std::array<uint8_t, Piece::kPiecesize> piece_count = {};
  for (const auto& piece : living_walls) {
    piece_count.at(piece.toUint8_t())++;
  }

  std::vector<bool> choose_pair = {false, false, false, false, true};
  std::shuffle(choose_pair.begin(), choose_pair.end(), g);
  std::uniform_int_distribution<> pair_chance(0, 3);
  std::uniform_int_distribution<> triplet_selection(0, kTriplets.size());
  std::uniform_int_distribution<> pair_selection(0, kPairs.size());

  std::vector<Piece> hand;
  for (bool choice : choose_pair) {
    if (choice) {
      int pair = pair_selection(g);
      while (piece_count.at(kPairs.at(pair)[0].toUint8_t()) < 2) {
        pair = pair_selection(g);
      }
      hand.push_back(kPairs.at(pair)[0]);
      hand.push_back(kPairs.at(pair)[1]);
      piece_count.at(kPairs.at(pair)[0].toUint8_t()) -= 2;
    } else {
      int triplet = triplet_selection(g);
      while ((triplet <= 20 &&
              (piece_count.at(kTriplets.at(triplet)[0].toUint8_t()) < 1 ||
               piece_count.at(kTriplets.at(triplet)[1].toUint8_t()) < 1 ||
               piece_count.at(kTriplets.at(triplet)[2].toUint8_t()) < 1)) ||
             (triplet > 20 &&
              piece_count.at(kTriplets.at(triplet)[0].toUint8_t()) < 3)) {
        triplet = triplet_selection(g);
      }
      if (triplet <= 20) {
        piece_count.at(kTriplets.at(triplet)[0].toUint8_t())--;
        piece_count.at(kTriplets.at(triplet)[1].toUint8_t())--;
        piece_count.at(kTriplets.at(triplet)[2].toUint8_t())--;
      } else {
        piece_count.at(kTriplets.at(triplet)[0].toUint8_t()) -= 3;
      }
      hand.push_back(kTriplets.at(triplet)[0]);
      hand.push_back(kTriplets.at(triplet)[1]);
      hand.push_back(kTriplets.at(triplet)[2]);
    }
  }
  return hand;
}

auto TestStdForm(std::vector<Piece> hand) -> bool {
  auto root = breakdownHand(std::move(hand));
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    bool complete = true;
    std::vector<const Node*> singles;
    for (const auto& node : branch) {
      if (node->type == Node::kSingle) {
        complete = false;
        break;
      }
    }
    if (complete) {
      return true;
    }
  }

  return false;
}

auto GetPossibleTenpaiHand(bool replacement) -> std::vector<Piece> {
  std::vector<Piece> tenpaihand = GetPossibleStdFormHand();
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(tenpaihand.begin(), tenpaihand.end(), g);
  std::uniform_int_distribution<> piece_index(0, 13);
  std::uniform_int_distribution<> piece_select(0, 33);
  int ind = piece_index(g);
  if (!replacement) {
    tenpaihand.erase(tenpaihand.begin() + ind);
    return tenpaihand;
  }
  tenpaihand[ind] = kPieceSet[piece_select(g)];
  while (TestStdForm(tenpaihand) && !TestValid(tenpaihand)) {
    tenpaihand[ind] = kPieceSet[piece_select(g)];
  }
  return tenpaihand;
}

}  // namespace mahjong
