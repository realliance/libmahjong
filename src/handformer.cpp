#include "handformer.h"

#include <ctype.h>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "piecetype.h"

using Mahjong::Piece;

std::map<char, Piece::Type> NOTATION_TO_SUIT = {
  {'m', Piece::CHARACTER_SUIT},
  {'s', Piece::BAMBOO_SUIT},
  {'p', Piece::PIN_SUIT},
  {'z', Piece::HONOR_SUIT},
};

std::map<uint8_t, Piece::Type> NOTATION_TO_HONOR = {
  {1, Piece::EAST_WIND},
  {2, Piece::SOUTH_WIND},
  {3, Piece::WEST_WIND},
  {4, Piece::NORTH_WIND},
  {5, Piece::WHITE_DRAGON},
  {6, Piece::GREEN_DRAGON},
  {7, Piece::RED_DRAGON},
};

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
      setSuit = NOTATION_TO_SUIT[c];
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
          return Piece(NOTATION_TO_HONOR[i]);
        }

        auto isRedFive = i == 0;
        auto value = isRedFive ? 5 : i;

        return Piece::formPiece(setSuit, value, isRedFive);
      });
    currentTiles.clear();
  }
  return result;
}
