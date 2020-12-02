#pragma once
#include <cstdint>
#include <string>

#include "winds.h"

namespace Mahjong {

class Piece {
 public:
  Piece() = default;
  //NOTLINTNEXTLINE(google-explicit-constructor)
  Piece(uint8_t p) : p(p) {}

  // TERMINAL_BIT, SUIT_2, RED_FIVE, PIECE_4
  enum Type : std::uint8_t {
    TERMINAL_BIT = 1 << 7,
    HONOR_SUIT = 0 << 5,
    BAMBOO_SUIT = 1 << 5,
    PIN_SUIT = 2 << 5,
    CHARACTER_SUIT = 3 << 5,
    RED_FIVE = 1 << 4,
    ERROR = 0,
    ONE_BAMBOO = BAMBOO_SUIT | 1 | TERMINAL_BIT,
    TWO_BAMBOO = BAMBOO_SUIT | 2,
    THREE_BAMBOO = BAMBOO_SUIT | 3,
    FOUR_BAMBOO = BAMBOO_SUIT | 4,
    FIVE_BAMBOO = BAMBOO_SUIT | 5,
    RED_FIVE_BAMBOO = BAMBOO_SUIT | 5 | RED_FIVE,
    SIX_BAMBOO = BAMBOO_SUIT | 6,
    SEVEN_BAMBOO = BAMBOO_SUIT | 7,
    EIGHT_BAMBOO = BAMBOO_SUIT | 8,
    NINE_BAMBOO = BAMBOO_SUIT | 9 | TERMINAL_BIT,
    ONE_PIN = PIN_SUIT | 1 | TERMINAL_BIT,
    TWO_PIN = PIN_SUIT | 2,
    THREE_PIN = PIN_SUIT | 3,
    FOUR_PIN = PIN_SUIT | 4,
    FIVE_PIN = PIN_SUIT | 5,
    RED_FIVE_PIN = PIN_SUIT | 5 | RED_FIVE,
    SIX_PIN = PIN_SUIT | 6,
    SEVEN_PIN = PIN_SUIT | 7,
    EIGHT_PIN = PIN_SUIT | 8,
    NINE_PIN = PIN_SUIT | 9 | TERMINAL_BIT,
    ONE_CHARACTER = CHARACTER_SUIT | 1 | TERMINAL_BIT,
    TWO_CHARACTER = CHARACTER_SUIT | 2,
    THREE_CHARACTER = CHARACTER_SUIT | 3,
    FOUR_CHARACTER = CHARACTER_SUIT | 4,
    FIVE_CHARACTER = CHARACTER_SUIT | 5,
    RED_FIVE_CHARACTER = CHARACTER_SUIT | 5 | RED_FIVE,
    SIX_CHARACTER = CHARACTER_SUIT | 6,
    SEVEN_CHARACTER = CHARACTER_SUIT | 7,
    EIGHT_CHARACTER = CHARACTER_SUIT | 8,
    NINE_CHARACTER = CHARACTER_SUIT | 9 | TERMINAL_BIT,
    EAST_WIND = HONOR_SUIT | 1,
    SOUTH_WIND = HONOR_SUIT | 2,
    WEST_WIND = HONOR_SUIT | 3,
    NORTH_WIND = HONOR_SUIT | 4,
    RED_DRAGON = HONOR_SUIT | 5,
    WHITE_DRAGON = HONOR_SUIT | 6,
    GREEN_DRAGON = HONOR_SUIT | 7
  };
  static const int PIECESIZE = 256;

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

  inline auto operator++() -> Piece& {
    p = p + 1;
    return *this;
  }

  inline auto operator==(const Piece rhs) const -> bool {
    return (p | RED_FIVE) == (rhs.p | RED_FIVE);
  }

  inline auto operator!=(const Piece rhs) const -> bool {
    return (p | RED_FIVE) != (rhs.p | RED_FIVE);
  }

  inline auto operator-(uint8_t i) const -> Piece {
    if (i == 0) {
      return Piece(p);
    }
    if (isHonor() || (int(getPieceNum()) - int(i) < 1)) {
      return Piece(ERROR);
    }
    if (getPieceNum() - i == 1) {
      return Piece(((p - i) & ~uint8_t(RED_FIVE)) | TERMINAL_BIT);
    }
    return Piece(((p - i) & ~uint8_t(RED_FIVE)) & ~uint8_t(TERMINAL_BIT));
  }

  inline auto operator+(uint8_t i) const -> Piece {
    if (i == 0) {
      return Piece(p);
    }
    if (isHonor() || (getPieceNum() + i > 9)) {
      return Piece(ERROR);
    }
    if (getPieceNum() + i == 9) {
      return Piece(((p + i) & ~uint8_t(RED_FIVE)) | TERMINAL_BIT);
    }
    return Piece(((p + i) & ~uint8_t(RED_FIVE)) & ~uint8_t(TERMINAL_BIT));
  }

  inline auto operator<(Piece other) const -> bool {
    if (getSuit() != other.getSuit()) {
      return getSuit() > other.getSuit();
    }
    return getPieceNum() < other.getPieceNum();
  };

 private:
  uint8_t p{ERROR};
};
}  // namespace Mahjong
