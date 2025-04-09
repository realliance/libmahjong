#pragma once
#include <cstdint>
#include <string>

#include "winds.h"

namespace mahjong {

class Piece {
 public:
  Piece() = default;
  constexpr explicit Piece(uint8_t p) : p_(p) {}

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

  [[nodiscard]] auto isHonor() const -> bool;
  [[nodiscard]] auto isTerminal() const -> bool;
  [[nodiscard]] auto isGreen() const -> bool;
  [[nodiscard]] auto isRedFive() const -> bool;
  [[nodiscard]] auto isBoardPiece() const -> bool;
  [[nodiscard]] auto toUint8_t() const -> uint8_t;
  [[nodiscard]] auto raw_value() const -> uint8_t;
  [[nodiscard]] auto getSuit() const -> uint8_t;
  [[nodiscard]] auto getPieceNum() const -> uint8_t;
  [[nodiscard]] auto toStr() const -> std::string;

  static auto fromWind(Wind w) -> Piece;
  static auto formPiece(Piece::Type suit, uint8_t number,
                        bool isRedFive = false) -> Piece;

  auto operator++() -> Piece& {
    p_ = p_ + 1;
    return *this;
  }

  auto operator==(const Piece rhs) const -> bool {
    return (p_ | kRedFive) == (rhs.p_ | kRedFive);
  }

  auto operator!=(const Piece rhs) const -> bool {
    return (p_ | kRedFive) != (rhs.p_ | kRedFive);
  }

  auto operator-(uint8_t i) const -> Piece {
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

  auto operator+(uint8_t i) const -> Piece {
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
