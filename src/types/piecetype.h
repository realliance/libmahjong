#pragma once
#include <cstdint>
#include <functional>
#include <string>

#include "winds.h"

namespace mahjong {

class Piece {
 public:
  Piece() = default;
  constexpr explicit Piece(uint8_t p) : p_(p) {}
  
  constexpr Piece(const Piece& p) = default;
  Piece& operator=(Piece&& p) = default;
  Piece& operator=(const Piece& p) = default;

  // TERMINAL_BIT, SUIT_2, RED_FIVE, PIECE_4
  enum Type : std::uint8_t {
    kTerminalBit = 1 << 7,
    kHonorSuit = 0 << 5,
    kBambooSuit = 1 << 5,
    kPinSuit = 2 << 5,
    kCharacterSuit = 3 << 5,
    kRedFive = 1 << 4,
    kError = 0,
    kOneBamboo = kBambooSuit | 1 | kTerminalBit,
    kTwoBamboo = kBambooSuit | 2,
    kThreeBamboo = kBambooSuit | 3,
    kFourBamboo = kBambooSuit | 4,
    kFiveBamboo = kBambooSuit | 5,
    kRedFiveBamboo = kBambooSuit | 5 | kRedFive,
    kSixBamboo = kBambooSuit | 6,
    kSevenBamboo = kBambooSuit | 7,
    kEightBamboo = kBambooSuit | 8,
    kNineBamboo = kBambooSuit | 9 | kTerminalBit,
    kOnePin = kPinSuit | 1 | kTerminalBit,
    kTwoPin = kPinSuit | 2,
    kThreePin = kPinSuit | 3,
    kFourPin = kPinSuit | 4,
    kFivePin = kPinSuit | 5,
    kRedFivePin = kPinSuit | 5 | kRedFive,
    kSixPin = kPinSuit | 6,
    kSevenPin = kPinSuit | 7,
    kEightPin = kPinSuit | 8,
    kNinePin = kPinSuit | 9 | kTerminalBit,
    kOneCharacter = kCharacterSuit | 1 | kTerminalBit,
    kTwoCharacter = kCharacterSuit | 2,
    kThreeCharacter = kCharacterSuit | 3,
    kFourCharacter = kCharacterSuit | 4,
    kFiveCharacter = kCharacterSuit | 5,
    kRedFiveCharacter = kCharacterSuit | 5 | kRedFive,
    kSixCharacter = kCharacterSuit | 6,
    kSevenCharacter = kCharacterSuit | 7,
    kEightCharacter = kCharacterSuit | 8,
    kNineCharacter = kCharacterSuit | 9 | kTerminalBit,
    kEastWind = kHonorSuit | 1,
    kSouthWind = kHonorSuit | 2,
    kWestWind = kHonorSuit | 3,
    kNorthWind = kHonorSuit | 4,
    kRedDragon = kHonorSuit | 5,
    kWhiteDragon = kHonorSuit | 6,
    kGreenDragon = kHonorSuit | 7
  };

  constexpr explicit Piece(Type type) : p_(type) {}

  static const int kPiecesize = 256;

  [[nodiscard]] bool isHonor() const;
  [[nodiscard]] bool isTerminal() const;
  [[nodiscard]] bool isGreen() const;
  [[nodiscard]] bool isRedFive() const;
  [[nodiscard]] bool isBoardPiece() const;
  [[nodiscard]] uint8_t toUint8_t() const;
  [[nodiscard]] uint8_t raw_value() const;
  [[nodiscard]] uint8_t getSuit() const;
  [[nodiscard]] uint8_t getPieceNum() const;
  [[nodiscard]] std::string toStr() const;

  static Piece fromWind(Wind w);
  static Piece formPiece(Piece::Type suit, uint8_t number,
                         bool isRedFive = false);

  Piece& operator++() {
    p_ = p_ + 1;
    return *this;
  }

  bool operator==(const Piece rhs) const {
    return (p_ | kRedFive) == (rhs.p_ | kRedFive);
  }

  bool operator!=(const Piece rhs) const {
    return (p_ | kRedFive) != (rhs.p_ | kRedFive);
  }

  Piece operator-(uint8_t i) const {
    if (i == 0) {
      return Piece(p_);
    }
    if (isHonor() ||
        (static_cast<int>(getPieceNum()) - static_cast<int>(i) < 1)) {
      return Piece(kError);
    }
    if (getPieceNum() - i == 1) {
      return Piece(((p_ - i) & ~static_cast<uint8_t>(kRedFive)) | kTerminalBit);
    }
    return Piece(((p_ - i) & ~static_cast<uint8_t>(kRedFive)) &
                 ~static_cast<uint8_t>(kTerminalBit));
  }

  Piece operator+(uint8_t i) const {
    if (i == 0) {
      return Piece(p_);
    }
    if (isHonor() || (getPieceNum() + i > 9)) {
      return Piece(kError);
    }
    if (getPieceNum() + i == 9) {
      return Piece(((p_ + i) & ~static_cast<uint8_t>(kRedFive)) | kTerminalBit);
    }
    return Piece(((p_ + i) & ~static_cast<uint8_t>(kRedFive)) &
                 ~static_cast<uint8_t>(kTerminalBit));
  }

  auto operator<=>(const Piece&) const = default;

 private:
  uint8_t p_{kError};
};

}  // namespace mahjong

// Custom specialization of std::hash can be injected in namespace std.
template <>
struct std::hash<mahjong::Piece> {
  std::size_t operator()(const mahjong::Piece& p) const noexcept {
    return std::hash<uint8_t>{}(p.raw_value());
  }
};
