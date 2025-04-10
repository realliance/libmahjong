#include "piecetype.h"

#include <map>
#include <string>
#include <utility>

#include "types/winds.h"

namespace mahjong {

uint8_t Piece::toUint8_t() const {
  return p_ & ~static_cast<uint8_t>(kRedFive);
}

uint8_t Piece::raw_value() const {
  return p_;
}

bool Piece::isHonor() const {
  return (p_ & kCharacterSuit) == kHonorSuit;
}

bool Piece::isTerminal() const {
  return (p_ & kTerminalBit) != kError;
}

bool Piece::isBoardPiece() const {
  return (p_ & 0x0F) != kError;
}

bool Piece::isRedFive() const {
  return (p_ & kRedFive) != kError;
}

uint8_t Piece::getSuit() const {
  return p_ & 3 << 5;
}

uint8_t Piece::getPieceNum() const {
  return p_ & 15;
}

bool Piece::isGreen() const {
  if (p_ == kGreenDragon) {
    return true;
  }
  if (getSuit() != kBambooSuit) {
    return false;
  }
  if (((getPieceNum() % 2) == 1) && getPieceNum() != 3) {
    return false;
  }
  return true;
}

Piece Piece::fromWind(Wind w) {
  switch (w) {
    case kEast:
      return Piece(kEastWind);
    case kSouth:
      return Piece(kSouthWind);
    case kWest:
      return Piece(kWestWind);
    case kNorth:
      return Piece(kNorthWind);
    default:
      return Piece(kError);
  }
}

Piece Piece::formPiece(Piece::Type suit, uint8_t number, bool isRedFive) {
  if (suit == kHonorSuit) {
    if (number < 1 || number > 7) {
      return Piece(kError);
    }

    return Piece(suit | number);
  }

  if (number < 1 || number > 9) {
    return Piece(kError);
  }

  auto value_append = number;
  if (isRedFive) {
    if (number != 5) {
      return Piece(kError);
    }
    value_append |= kRedFive;
  }

  if (number == 1 || number == 9) {
    value_append |= kTerminalBit;
  }

  if (suit == kCharacterSuit || suit == kPinSuit || suit == kBambooSuit) {
    return Piece(suit | value_append);
  }

  return Piece(kError);
}

const std::map<uint8_t, std::string> kStrMap = {
    {Piece::kError, "ERRORPIECE"},
    {Piece::kRedDragon, "Red Dragon"},
    {Piece::kWhiteDragon, "White Dragon"},
    {Piece::kGreenDragon, "Green Dragon"},
    {Piece::kOneCharacter, "Man1"},
    {Piece::kTwoCharacter, "Man2"},
    {Piece::kThreeCharacter, "Man3"},
    {Piece::kFourCharacter, "Man4"},
    {Piece::kRedFiveCharacter, "Man5-Dora"},
    {Piece::kFiveCharacter, "Man5"},
    {Piece::kSixCharacter, "Man6"},
    {Piece::kSevenCharacter, "Man7"},
    {Piece::kEightCharacter, "Man8"},
    {Piece::kNineCharacter, "Man9"},
    {Piece::kSouthWind, "South"},
    {Piece::kNorthWind, "North"},
    {Piece::kOnePin, "Pin1"},
    {Piece::kTwoPin, "Pin2"},
    {Piece::kThreePin, "Pin3"},
    {Piece::kFourPin, "Pin4"},
    {Piece::kRedFivePin, "Pin5-Dora"},
    {Piece::kFivePin, "Pin5"},
    {Piece::kSixPin, "Pin6"},
    {Piece::kSevenPin, "Pin7"},
    {Piece::kEightPin, "Pin8"},
    {Piece::kNinePin, "Pin9"},
    {Piece::kWestWind, "West"},
    {Piece::kOneBamboo, "Sou1"},
    {Piece::kTwoBamboo, "Sou2"},
    {Piece::kThreeBamboo, "Sou3"},
    {Piece::kFourBamboo, "Sou4"},
    {Piece::kRedFiveBamboo, "Sou5-Dora"},
    {Piece::kFiveBamboo, "Sou5"},
    {Piece::kSixBamboo, "Sou6"},
    {Piece::kSevenBamboo, "Sou7"},
    {Piece::kEightBamboo, "Sou8"},
    {Piece::kNineBamboo, "Sou9"},
    {Piece::kEastWind, "East"},
};

std::string Piece::toStr() const {
  if (kStrMap.contains(p_)) {
    return kStrMap.at(p_);
  }
  return kStrMap.at(0);  // ERRORPIECE
}
}  // namespace mahjong
