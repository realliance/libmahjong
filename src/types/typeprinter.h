#pragma once

#include <ostream>

namespace mahjong {
struct Event;
struct GameState;
struct Hand;
class Node;
struct Meld;
class Walls;
struct Player;
}  // namespace mahjong

std::ostream& operator<<(std::ostream& os, const mahjong::Event& e);

std::ostream& operator<<(std::ostream& os, const mahjong::GameState& state);

std::ostream& operator<<(std::ostream& os, const mahjong::Hand& hand);

std::ostream& operator<<(std::ostream& os, const mahjong::Node& node);

std::ostream& operator<<(std::ostream& os, const mahjong::Meld& meld);

std::ostream& operator<<(std::ostream& os, const mahjong::Walls& walls);

std::ostream& operator<<(std::ostream& os, const mahjong::Player& player);
