#include "gamestate.h"

#include <cstddef>
#include <string>

#include "hand.h"
#include "player.h"

auto operator<<(std::ostream& os, const mahjong::GameState& state)
    -> std::ostream& {
  os << "currentPlayer: " << state.currentPlayer << '\n';
  os << "turnNum: " << state.turnNum << '\n';
  os << "roundNum: " << state.roundNum << '\n';
  os << "riichiSticks: " << state.riichiSticks << '\n';
  os << "counters: " << state.counters << '\n';
  os << "lastCall: " << state.lastCall << '\n';
  os << "lastCaller: " << state.lastCaller << '\n';
  os << "seed: " << state.seed << '\n';
  os << "pendingPiece: " << state.pendingPiece.toStr() << '\n';
  os << "hasRonned: " << '\n';
  for (size_t i = 0; i < state.hasRonned.size(); i++) {
    os << "hasRonned[" << i << "]: {" << state.hasRonned.at(i) << "}" << '\n';
  }
  os << "hands: " << '\n';
  for (size_t i = 0; i < state.hands.size(); i++) {
    os << "hand[" << i << "]: {" << state.hands.at(i) << "}" << '\n';
  }
  os << "players: " << '\n';
  for (size_t i = 0; i < state.players.size(); i++) {
    os << "player[" << i << "]: " << state.players.at(i) << '\n';
  }
  os << state.walls << '\n';
  return os;
}
