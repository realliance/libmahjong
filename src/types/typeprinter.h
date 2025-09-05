#pragma once

#include <ostream>

namespace mahjong {
struct Event;
struct GameState;
struct Player;
class Node;
struct Meld;
class Walls;
struct Hand;
}  // namespace mahjong

std::ostream& operator<<(std::ostream& os, const mahjong::Event& e);

std::ostream& operator<<(std::ostream& os, const mahjong::GameState& state);

std::ostream& operator<<(std::ostream& os, const mahjong::Hand& player);

std::ostream& operator<<(std::ostream& os, const mahjong::Node& node);

std::ostream& operator<<(std::ostream& os, const mahjong::Meld& meld);

std::ostream& operator<<(std::ostream& os, const mahjong::Walls& walls);
