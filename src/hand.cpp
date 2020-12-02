#include "hand.h"

#include <ostream>
#include <string>

using Mahjong::Hand;

auto operator<<(std::ostream& os, const Hand& hand) -> std::ostream& {
  os << "{ open: " << hand.open;
  os << " riichi: " << hand.riichi;
  os << " riichiPieceDiscard: " << hand.riichiPieceDiscard;
  os << " riichiRound: " << hand.riichiRound;
  os << " live: [" << std::endl;
  ;
  for (const auto& piece : hand.live) {
    os << piece.toStr() << ", ";
  }
  os << "]" << std::endl;
  os << "melds: [" << std::endl;
  for (const auto& meld : hand.melds) {
    os << meld << ", ";
  }
  os << "]" << std::endl;
  os << "discards: [" << std::endl;
  for (const auto& piece : hand.discards) {
    os << piece.toStr() << ", ";
  }
  os << "]" << std::endl;
  os << "}";
  return os;
}
