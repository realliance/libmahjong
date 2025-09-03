#include "utils/handformer.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iterator>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"

namespace mahjong {

const std::unordered_map<char, Piece::Type> kNotationToSuit = {
    {'m', Piece::Type::kCharacterSuit},
    {'s', Piece::Type::kBambooSuit},
    {'p', Piece::Type::kPinSuit},
    {'z', Piece::Type::kHonorSuit},
};

const std::unordered_map<Piece::Type, char> kSuitToNotation = {
    {Piece::Type::kCharacterSuit, 'm'},
    {Piece::Type::kBambooSuit, 's'},
    {Piece::Type::kPinSuit, 'p'},
    {Piece::Type::kHonorSuit, 'z'},
};

const std::unordered_map<uint8_t, Piece::Type> kNotationToHonor = {
    {1, Piece::Type::kEastWind},    {2, Piece::Type::kSouthWind},
    {3, Piece::Type::kWestWind},    {4, Piece::Type::kNorthWind},
    {5, Piece::Type::kWhiteDragon}, {6, Piece::Type::kGreenDragon},
    {7, Piece::Type::kRedDragon},
};

/**
 * HandFromNotation will attempt to produce as much of the hand as possible.
 * Use IsValidNotation to guarantee a successful input.
 */
std::vector<Piece> HandFromNotation(const std::string& notation) {
  std::vector<Piece> result;
  std::vector<int8_t> current_tiles;
  for (const auto& c : notation) {
    if (!std::isdigit(c) && !std::isalpha(c)) {
      // Push -1 if input is invalid (which is later translated to an error
      // piece)
      current_tiles.push_back(-1);
      continue;
    }

    if (std::isdigit(c)) {
      // Converts character digit to integer number (safe with UTF-7 and UTF-8)
      auto value = c - '0';
      current_tiles.push_back(value);
      continue;
    }

    // Input must be alphabetical, translate queued set
    auto set_suit = Piece::Type::kError;
    auto is_error = true;
    if (kNotationToSuit.contains(c)) {
      set_suit = kNotationToSuit.at(c);
      is_error = false;
    }
    auto is_honor = Piece(set_suit).isHonor();
    std::ranges::transform(
        current_tiles,

        std::back_inserter(result),
        [&set_suit, &is_honor, &is_error](int8_t i) -> Piece {
          if (is_error || i == -1) {
            return kError;
          }

          if (is_honor) {
            return Piece(kNotationToHonor.at(i));
          }

          auto is_red_five = i == 0;
          auto value = is_red_five ? 5 : i;

          return Piece::formPiece(set_suit, value, is_red_five);
        });
    current_tiles.clear();
  }
  // Free Remaining tiles (in case of no ending suit)
  current_tiles.clear();

  return result;
}

void HandFromNotation(const std::string& notation, Hand* hand) {
  std::vector hand_vec = HandFromNotation(notation);
  hand->live_count = hand_vec.size();
  std::ranges::move(hand_vec, hand->live.begin());
}

bool IsValidNotation(const std::string& notation) {
  std::vector<uint8_t> current_set;
  for (const auto& c : notation) {
    // If symbol, not valid
    if (!std::isdigit(c) && !std::isalpha(c)) {
      return false;
    }

    if (std::isdigit(c)) {
      // Converts character digit to integer number (safe with UTF-7 and UTF-8)
      auto value = c - '0';
      current_set.push_back(value);
      continue;
    }

    // Suit Found, check if valid
    if (!kNotationToSuit.contains(c)) {
      return false;
    }

    if (c == 'z') {
      for (const auto& i : current_set) {
        if (i > 7) {
          return false;
        }
      }
    }

    current_set.clear();
  }

  // Check if notation ended with suit for final set.
  return current_set.empty();
}

std::string HandToNotation(const std::vector<Piece>& hand) {
  std::map<Piece::Type, std::vector<uint8_t>> suit_and_tiles;

  for (const auto& t : hand) {
    if (t == kError) {
      continue;
    }

    const auto suit = static_cast<Piece::Type>(t.getSuit());
    if (t.isRedFive()) {
      suit_and_tiles[suit].push_back(0);
      continue;
    }

    suit_and_tiles[suit].push_back(t.getPieceNum());
  }

  std::string result;
  for (const auto& pair : suit_and_tiles) {
    std::string number_list;
    for (const auto& i : pair.second) {
      number_list.append(std::to_string(i));
    }
    result.append(number_list);
    result.push_back(kSuitToNotation.at(pair.first));
  }

  return result;
}
}  // namespace mahjong
