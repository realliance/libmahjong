#include "hand.h"

#include <ostream>
#include <string>

std::ostream& operator<<(std::ostream& os, const mahjong::Hand& hand) {
  os << "{ open: " << hand.open;
  os << " riichi: " << hand.riichi;
  os << " riichiPieceDiscard: " << hand.riichiPieceDiscard;
  os << " riichiRound: " << hand.riichiRound;
  os << " live: [" << '\n';
  ;
  for (const auto& piece : hand.live) {
    os << piece.toStr() << ", ";
  }
  os << "]" << '\n';
  os << "melds: [" << '\n';
  for (const auto& meld : hand.melds) {
    os << meld << ", ";
  }
  os << "]" << '\n';
  os << "discards: [" << '\n';
  for (const auto& piece : hand.discards) {
    os << piece.toStr() << ", ";
  }
  os << "]" << '\n';
  os << "}";
  return os;
}
