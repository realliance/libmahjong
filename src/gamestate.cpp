#include "gamestate.h"

#include <cstddef>
#include <string>

#include "hand.h"
#include "player.h"

using Mahjong::GameState;

auto operator<<(std::ostream& os, const GameState& state) -> std::ostream& {
  os << "currentPlayer: " << state.currentPlayer << std::endl;
  os << "turnNum: " << state.turnNum << std::endl;
  os << "roundNum: " << state.roundNum << std::endl;
  os << "riichiSticks: " << state.riichiSticks << std::endl;
  os << "counters: " << state.counters << std::endl;
  os << "lastCall: " << state.lastCall << std::endl;
  os << "lastCaller: " << state.lastCaller << std::endl;
  os << "seed: " << state.seed << std::endl;
  os << "pendingPiece: " << state.pendingPiece.toStr() << std::endl;
  os << "hasRonned: " << std::endl;
  for (size_t i = 0; i < state.hasRonned.size(); i++) {
    os << "hasRonned[" << i << "]: {" << state.hasRonned.at(i) << "}" << std::endl;
  }
  os << "hands: " << std::endl;
  for (size_t i = 0; i < state.hands.size(); i++) {
    os << "hand[" << i << "]: {" << state.hands.at(i) << "}" << std::endl;
  }
  os << "players: " << std::endl;
  for (size_t i = 0; i < state.players.size(); i++) {
    os << "player[" << i << "]: " << state.players.at(i) << std::endl;
  }
  os << state.walls << std::endl;
  return os;
}
