#include "walls.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <string>

#include "pieces.h"

namespace mahjong {

const std::vector<Piece> kPieceSet = {
    kOneBamboo,    kTwoBamboo,      kThreeBamboo,    kFourBamboo,
    kFiveBamboo,   kSixBamboo,      kSevenBamboo,    kEightBamboo,
    kNineBamboo,   kOnePin,         kTwoPin,         kThreePin,
    kFourPin,      kFivePin,        kSixPin,         kSevenPin,
    kEightPin,     kNinePin,        kOneCharacter,   kNineCharacter,
    kTwoCharacter, kThreeCharacter, kFourCharacter,  kFiveCharacter,
    kSixCharacter, kSevenCharacter, kEightCharacter, kWhiteDragon,
    kGreenDragon,  kRedDragon,      kEastWind,       kSouthWind,
    kNorthWind,    kWestWind};

Walls::Walls() {
  std::random_device rd;
  std::mt19937 g(rd());

  for (int i = 0; i < 4; i++) {
    livingWalls.insert(livingWalls.end(), kPieceSet.begin(), kPieceSet.end());
  }
  std::shuffle(livingWalls.begin(), livingWalls.end(), g);

  std::move(livingWalls.begin(), livingWalls.begin() + 14,
            std::back_inserter(deadWall));
  for (size_t i = 0; i < 14; i++) {
    livingWalls.erase(livingWalls.begin());
  }
}

Walls::Walls(std::mt19937& g) {
  std::vector<Piece> wall;
  for (int i = 0; i < 4; i++) {
    livingWalls.insert(livingWalls.end(), kPieceSet.begin(), kPieceSet.end());
  }
  std::shuffle(livingWalls.begin(), livingWalls.end(), g);

  std::move(livingWalls.begin(), livingWalls.begin() + 14,
            std::back_inserter(deadWall));
  for (size_t i = 0; i < 14; i++) {
    livingWalls.erase(livingWalls.begin());
  }
}

Walls::Walls(std::vector<Piece> wall) {
  std::swap(livingWalls, wall);
  std::move(livingWalls.rbegin(), livingWalls.rbegin() + 14,
            std::back_inserter(deadWall));
  for (size_t i = 0; i < 14; i++) {
    livingWalls.pop_back();
  }
}

Piece Walls::TakePiece() {
  if (!livingWalls.empty()) {
    Piece p = livingWalls.front();
    livingWalls.erase(livingWalls.begin());
    return p;
  }
  return kError;
}

std::vector<Piece> Walls::TakeHand() {
  if (livingWalls.size() < 13) {
    return {};
  }
  std::vector<Piece> hand;
  std::move(livingWalls.begin(), livingWalls.begin() + 13,
            std::back_inserter(hand));
  for (size_t i = 0; i < 13; i++) {
    livingWalls.erase(livingWalls.begin());
  }
  return hand;
}

Piece Walls::TakeReplacementTile() {
  if (livingWalls.empty()) {
    return kError;
  }
  if (replacements < 1) {
    return kError;
  }
  replacements--;
  Piece p = deadWall.front();
  deadWall.erase(deadWall.begin());
  deadWall.push_back(livingWalls.back());
  doraCount++;
  livingWalls.pop_back();
  return p;
}

std::vector<Piece> Walls::GetDoras() const {
  std::vector<Piece> doras;
  std::copy_n(deadWall.begin() + replacements, doraCount,
              std::back_inserter(doras));
  return doras;
}

std::vector<Piece> Walls::GetUraDoras() const {
  std::vector<Piece> doras;
  std::copy_n(deadWall.begin() + replacements + doraCount, doraCount,
              std::back_inserter(doras));
  return doras;
}

int Walls::GetRemainingPieces() const {
  return livingWalls.size();
}

}  // namespace mahjong
