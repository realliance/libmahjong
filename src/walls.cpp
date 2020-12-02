#include "walls.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <string>

using Mahjong::Piece, Mahjong::Walls;

const std::vector<Piece> PIECE_SET = {
  Piece::ONE_BAMBOO, Piece::TWO_BAMBOO, Piece::THREE_BAMBOO, Piece::FOUR_BAMBOO,
  Piece::FIVE_BAMBOO, Piece::SIX_BAMBOO, Piece::SEVEN_BAMBOO, Piece::EIGHT_BAMBOO,
  Piece::NINE_BAMBOO, Piece::ONE_PIN, Piece::TWO_PIN, Piece::THREE_PIN, Piece::FOUR_PIN,
  Piece::FIVE_PIN, Piece::SIX_PIN, Piece::SEVEN_PIN, Piece::EIGHT_PIN, Piece::NINE_PIN,
  Piece::ONE_CHARACTER, Piece::NINE_CHARACTER, Piece::TWO_CHARACTER,
  Piece::THREE_CHARACTER, Piece::FOUR_CHARACTER, Piece::FIVE_CHARACTER,
  Piece::SIX_CHARACTER, Piece::SEVEN_CHARACTER, Piece::EIGHT_CHARACTER,
  Piece::WHITE_DRAGON, Piece::GREEN_DRAGON, Piece::RED_DRAGON, Piece::EAST_WIND,
  Piece::SOUTH_WIND, Piece::NORTH_WIND, Piece::WEST_WIND};

Walls::Walls() {
  std::random_device rd;
  std::mt19937 g(rd());

  for (int i = 0; i < 4; i++) {
    livingWalls.insert(livingWalls.end(), PIECE_SET.begin(), PIECE_SET.end());
  }
  std::shuffle(livingWalls.begin(), livingWalls.end(), g);

  std::move(livingWalls.begin(), livingWalls.begin() + 14, std::back_inserter(deadWall));
  for (size_t i = 0; i < 14; i++) {
    livingWalls.erase(livingWalls.begin());
  }
}

Walls::Walls(std::mt19937& g) {
  std::vector<Piece> wall;
  for (int i = 0; i < 4; i++) {
    livingWalls.insert(livingWalls.end(), PIECE_SET.begin(), PIECE_SET.end());
  }
  std::shuffle(livingWalls.begin(), livingWalls.end(), g);

  std::move(livingWalls.begin(), livingWalls.begin() + 14, std::back_inserter(deadWall));
  for (size_t i = 0; i < 14; i++) {
    livingWalls.erase(livingWalls.begin());
  }
}

Walls::Walls(std::vector<Piece> wall) {
  std::swap(livingWalls, wall);
  std::move(livingWalls.rbegin(), livingWalls.rbegin() + 14, std::back_inserter(deadWall));
  for (size_t i = 0; i < 14; i++) {
    livingWalls.pop_back();
  }
}

auto Walls::TakePiece() -> Piece {
  if (!livingWalls.empty()) {
    Piece p = livingWalls.front();
    livingWalls.erase(livingWalls.begin());
    return p;
  }
  return Piece::ERROR;
}

auto Walls::TakeHand() -> std::vector<Piece> {
  if (livingWalls.size() < 13) {
    return {};
  }
  std::vector<Piece> hand;
  std::move(livingWalls.begin(), livingWalls.begin() + 13, std::back_inserter(hand));
  for (size_t i = 0; i < 13; i++) {
    livingWalls.erase(livingWalls.begin());
  }
  return hand;
}

auto Walls::TakeReplacementTile() -> Piece {
  if (livingWalls.empty()) {
    return Piece::ERROR;
  }
  if (replacements < 1) {
    return Piece::ERROR;
  }
  replacements--;
  Piece p = deadWall.front();
  deadWall.erase(deadWall.begin());
  deadWall.push_back(livingWalls.back());
  doraCount++;
  livingWalls.pop_back();
  return p;
}

auto Walls::GetDoras() const -> std::vector<Piece> {
  std::vector<Piece> doras;
  std::copy_n(deadWall.begin() + replacements, doraCount, std::back_inserter(doras));
  return doras;
}

auto Walls::GetUraDoras() const -> std::vector<Piece> {
  std::vector<Piece> doras;
  std::copy_n(deadWall.begin() + replacements + doraCount, doraCount, std::back_inserter(doras));
  return doras;
}

auto Walls::GetRemainingPieces() const -> int {
  return livingWalls.size();
}

auto operator<<(std::ostream& os, const Mahjong::Walls& walls) -> std::ostream& {
  os << "{ doraCount: " << walls.doraCount;
  os << " replacements: " << walls.replacements;
  os << " livingWalls: [" << std::endl;
  for (const auto& piece : walls.livingWalls) {
    os << piece.toStr() << ", ";
  }
  os << "]" << std::endl;
  os << " deadWall: [" << std::endl;
  for (const auto& piece : walls.deadWall) {
    os << piece.toStr() << ", ";
  }
  os << "]" << std::endl;
  os << "}";
  return os;
}
