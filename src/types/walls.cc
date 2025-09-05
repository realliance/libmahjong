#include "types/walls.h"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <vector>

#include "types/gamestate.h"
#include "types/hand.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/settings.h"

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

void Walls::New(GameState& state) {
  std::vector<Piece> living_walls;
  living_walls.reserve(kPieceSet.size() * 4);
  for (int i = 0; i < 4; i++) {
    living_walls.insert(living_walls.end(), kPieceSet.begin(), kPieceSet.end());
  }
  std::shuffle(living_walls.begin(), living_walls.end(), state.g);

  for (int i = 0; i < kLivingWallCount; ++i) {
    state.livingWall[i] = living_walls[i];
  }
  for (int i = 0; i < kDeadWallCount; ++i) {
    state.deadWall[i] = living_walls[kLivingWallCount + i];
  }
}

Piece Walls::TakePiece(GameState& state) {
  if (state.livingWallIndex == kLivingWallCount) {
    return kError;
  }
  const Piece p = state.livingWall[state.livingWallIndex++];
  return p;
}

std::vector<Piece> Walls::TakeHand(GameState& state) {
  if (state.livingWallIndex + 13 == kLivingWallCount) {
    return {};
  }
  std::vector<Piece> hand;
  hand.resize(13);
  for (size_t i = 0; i < 13; i++) {
    hand[i] = state.livingWall[state.livingWallIndex++];
  }
  return hand;
}

Piece Walls::TakeReplacementTile(GameState& state) {
  if (state.deadWallIndex == kReplacementCount) {
    return kError;
  }
  const Piece p = state.deadWall[state.deadWallIndex++];
  state.livingWallIndex++;
  state.doraCount++;
  return p;
}

std::vector<Piece> Walls::GetDoras(const GameState& state) {
  std::vector<Piece> doras;
  std::copy_n(state.deadWall.begin() + kReplacementCount, state.doraCount,
              std::back_inserter(doras));
  return doras;
}

std::vector<Piece> Walls::GetUraDoras(const GameState& state) {
  std::vector<Piece> doras;
  std::copy_n(state.deadWall.begin() + kReplacementCount + kDoraCount,
              state.doraCount, std::back_inserter(doras));
  return doras;
}

int Walls::GetRemainingPieces(const GameState& state) {
  return kLivingWallCount - state.livingWallIndex;
}

}  // namespace mahjong
