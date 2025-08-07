#include "typeprinter.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>
#include <string>

#include "controllers/playercontroller.h"
#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "handnode.h"
#include "meld.h"
#include "types/piecetype.h"
#include "types/player.h"
#include "walls.h"

std::ostream& operator<<(std::ostream& os, const mahjong::Event& e) {
  os << "{type: " << e.typeToStr();
  os << " player: " << e.player;
  const uint8_t p = e.piece;
  os << " piece: " << mahjong::Piece(p).toStr();
  os << " decision: " << e.decision << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const mahjong::GameState& state) {
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

std::ostream& operator<<(std::ostream& os, const mahjong::Node& node) {
  os << "{ id: " << node.id() << ", type:" << node.typeToStr();
  os << ", start:" << node.start().toStr();
  os << ", parent: "
     << (node.parent() != nullptr ? std::to_string(node.parent()->id())
                                  : "No Parent");
  os << ", leaves: [ ";
  for (const auto& leaf : node.leaves()) {
    os << "id: " << leaf->id() << ", ";
  }
  os << " ], "
     << "leafPosInParent: " << node.leafPosInParent() << " },";
  os << '\n';
  return os;
}

std::ostream& operator<<(std::ostream& os, const mahjong::Meld& meld) {
  os << "{ type: " << meld.typeToStr();
  os << ", start: " << meld.start.toStr() << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const mahjong::Walls& walls) {
  os << "{ doraCount: " << walls.doraCount;
  os << " replacements: " << walls.replacements;
  os << " livingWalls: [" << '\n';
  for (const auto& piece : walls.livingWalls) {
    os << piece.toStr() << ", ";
  }
  os << "]" << '\n';
  os << " deadWall: [" << '\n';
  for (const auto& piece : walls.deadWall) {
    os << piece.toStr() << ", ";
  }
  os << "]" << '\n';
  os << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const mahjong::Player& player) {
  os << "{ controller: "
     << ((player.controller != nullptr) ? player.controller->Name()
                                        : "NULLPTR");
  os << " points: " << player.points;
  return os;
}
