#include "piecetype.h"

#include <map>
#include <string>

using Mahjong::Piece;

auto Piece::toUint8_t() const -> uint8_t {
  return p & ~uint8_t(RED_FIVE);
}

auto Piece::raw_value() const -> uint8_t {
  return p;
}

auto Piece::isHonor() const -> bool {
  return (p & CHARACTER_SUIT) == HONOR_SUIT;
}

auto Piece::isTerminal() const -> bool {
  return (p & TERMINAL_BIT) != ERROR;
}

auto Piece::isBoardPiece() const -> bool {
  return (p & 0x0F) != ERROR;
}

auto Piece::isRedFive() const -> bool {
  return (p & RED_FIVE) != ERROR;
}

auto Piece::getSuit() const -> uint8_t {
  return p & 3 << 5;
}

auto Piece::getPieceNum() const -> uint8_t {
  return p & 15;
}

auto Piece::isGreen() const -> bool {
  if (p == GREEN_DRAGON) {
    return true;
  }
  if (getSuit() != BAMBOO_SUIT) {
    return false;
  }
  if (((getPieceNum() % 2) == 1) && getPieceNum() != 3) {
    return false;
  }
  return true;
}

auto Piece::fromWind(Wind w) -> Piece {
  switch (w) {
    case East:
      return Piece(EAST_WIND);
    case South:
      return Piece(SOUTH_WIND);
    case West:
      return Piece(WEST_WIND);
    case North:
      return Piece(NORTH_WIND);
    default:
      return Piece(ERROR);
  }
}

auto Piece::formPiece(Piece::Type suit, uint8_t number) -> Piece {
  auto valueAppend = number;
  if (number == 1 || number == 9) {
    valueAppend |= TERMINAL_BIT;
  } else if (number == 0) {
    valueAppend = 5 | RED_FIVE;
  }

  if (suit == CHARACTER_SUIT || suit == PIN_SUIT || suit == BAMBOO_SUIT || suit == HONOR_SUIT) {
    return Piece(suit | valueAppend);
  } else {
    return Piece(ERROR);
  }
}

const std::map<uint8_t, std::string> STR_MAP = {
  {Piece::ERROR, "ERRORPIECE"},
  {Piece::RED_DRAGON, "Red Dragon"},
  {Piece::WHITE_DRAGON, "White Dragon"},
  {Piece::GREEN_DRAGON, "Green Dragon"},
  {Piece::ONE_CHARACTER, "Man1"},
  {Piece::TWO_CHARACTER, "Man2"},
  {Piece::THREE_CHARACTER, "Man3"},
  {Piece::FOUR_CHARACTER, "Man4"},
  {Piece::RED_FIVE_CHARACTER, "Man5-Dora"},
  {Piece::FIVE_CHARACTER, "Man5"},
  {Piece::SIX_CHARACTER, "Man6"},
  {Piece::SEVEN_CHARACTER, "Man7"},
  {Piece::EIGHT_CHARACTER, "Man8"},
  {Piece::NINE_CHARACTER, "Man9"},
  {Piece::SOUTH_WIND, "South"},
  {Piece::NORTH_WIND, "North"},
  {Piece::ONE_PIN, "Pin1"},
  {Piece::TWO_PIN, "Pin2"},
  {Piece::THREE_PIN, "Pin3"},
  {Piece::FOUR_PIN, "Pin4"},
  {Piece::RED_FIVE_PIN, "Pin5-Dora"},
  {Piece::FIVE_PIN, "Pin5"},
  {Piece::SIX_PIN, "Pin6"},
  {Piece::SEVEN_PIN, "Pin7"},
  {Piece::EIGHT_PIN, "Pin8"},
  {Piece::NINE_PIN, "Pin9"},
  {Piece::WEST_WIND, "West"},
  {Piece::ONE_BAMBOO, "Sou1"},
  {Piece::TWO_BAMBOO, "Sou2"},
  {Piece::THREE_BAMBOO, "Sou3"},
  {Piece::FOUR_BAMBOO, "Sou4"},
  {Piece::RED_FIVE_BAMBOO, "Sou5-Dora"},
  {Piece::FIVE_BAMBOO, "Sou5"},
  {Piece::SIX_BAMBOO, "Sou6"},
  {Piece::SEVEN_BAMBOO, "Sou7"},
  {Piece::EIGHT_BAMBOO, "Sou8"},
  {Piece::NINE_BAMBOO, "Sou9"},
  {Piece::EAST_WIND, "East"},
};

auto Piece::toStr() const -> std::string {
  if (STR_MAP.contains(p)) {
    return STR_MAP.at(p);
  }
  return STR_MAP.at(0);  //ERRORPIECE
}
