#include "handformer.h"

#include <ctype.h>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "piecetype.h"

using Mahjong::Piece;

const std::map<char, Piece::Type> NOTATION_TO_SUIT = {
  {'m', Piece::CHARACTER_SUIT},
  {'s', Piece::BAMBOO_SUIT},
  {'p', Piece::PIN_SUIT},
  {'z', Piece::HONOR_SUIT},
};

const std::map<Piece::Type, char> SUIT_TO_NOTATION = {
  {Piece::CHARACTER_SUIT, 'm'},
  {Piece::BAMBOO_SUIT, 's'},
  {Piece::PIN_SUIT, 'p'},
  {Piece::HONOR_SUIT, 'z'},
};

const std::map<uint8_t, Piece::Type>
  NOTATION_TO_HONOR = {
    {1, Piece::EAST_WIND},
    {2, Piece::SOUTH_WIND},
    {3, Piece::WEST_WIND},
    {4, Piece::NORTH_WIND},
    {5, Piece::WHITE_DRAGON},
    {6, Piece::GREEN_DRAGON},
    {7, Piece::RED_DRAGON},
};

/**
 * HandFromNotation will attempt to produce as much of the hand as possible.
 * Use IsValidNotation to guarantee a successful input.
 */
auto Mahjong::HandFromNotation(std::string notation) -> std::vector<Piece> {
  std::vector<Piece> result;
  std::vector<int8_t> currentTiles;
  for (const auto& c : notation) {
    if (!std::isdigit(c) && !std::isalpha(c)) {
      // Push -1 if input is invalid (which is later translated to an error piece)
      currentTiles.push_back(-1);
      continue;
    }

    if (std::isdigit(c)) {
      // Converts character digit to integer number (safe with UTF-7 and UTF-8)
      auto value = c - '0';
      currentTiles.push_back(value);
      continue;
    }

    // Input must be alphabetical, translate queued set
    auto setSuit = Piece::ERROR;
    auto isError = true;
    if (NOTATION_TO_SUIT.count(c) > 0) {
      setSuit = NOTATION_TO_SUIT.at(c);
      isError = false;
    }
    auto isHonor = Piece(setSuit).isHonor();
    transform(
      currentTiles.begin(),
      currentTiles.end(),
      std::back_inserter(result),
      [&setSuit, &isHonor, &isError](int8_t i) -> Piece {
        if (isError || i == -1) {
          return Piece(Piece::ERROR);
        }

        if (isHonor) {
          return Piece(NOTATION_TO_HONOR.at(i));
        }

        auto isRedFive = i == 0;
        auto value = isRedFive ? 5 : i;

        return Piece::formPiece(setSuit, value, isRedFive);
      });
    currentTiles.clear();
  }
  // Free Remaining tiles (in case of no ending suit)
  currentTiles.clear();

  return result;
}

auto Mahjong::IsValidNotation(std::string notation) -> bool {
  std::vector<uint8_t> currentSet;
  for (const auto& c : notation) {
    // If symbol, not valid
    if (!std::isdigit(c) && !std::isalpha(c)) {
      return false;
    }

    if (std::isdigit(c)) {
      // Converts character digit to integer number (safe with UTF-7 and UTF-8)
      auto value = c - '0';
      currentSet.push_back(value);
      continue;
    }

    // Suit Found, check if valid
    if (NOTATION_TO_SUIT.count(c) == 0) {
      return false;
    }

    if (c == 'z') {
      for (const auto& i : currentSet) {
        if (i > 7) {
          return false;
        }
      }
    }

    currentSet.clear();
  }

  // Check if notation ended with suit for final set.
  if (currentSet.size() > 0) {
    return false;
  }

  return true;
}

auto Mahjong::HandToNotation(std::vector<Piece> hand) -> std::string {
  std::map<Piece::Type, std::vector<uint8_t>> suitAndTiles;

  for (const auto& t : hand) {
    if (t == Piece(Piece::ERROR)) {
      continue;
    }

    const auto suit = (Piece::Type)t.getSuit();
    if (t.isRedFive()) {
      suitAndTiles[suit].push_back(0);
      continue;
    }

    suitAndTiles[suit].push_back(t.getPieceNum());
  }

  std::string result;
  for (const auto& pair : suitAndTiles) {
    std::string numberList;
    for (const auto& i : pair.second) {
      numberList.append(std::to_string(i));
    }
    result.append(numberList);
    result.push_back(SUIT_TO_NOTATION.at(pair.first));
  }

  return result;
}
