#include "types/typeprinter.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>
#include <string>

#include "analysis/handnode.h"
#include "controllers/playercontroller.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"

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
    os << "player[" << i << "]: ";
    os << "{ controller: "
       << ((state.players[i] != nullptr) ? state.players[i]->Name()
                                         : "NULLPTR");
    os << " points: " << state.points[i];
    os << '\n';
  }
  os << "{ doraCount: " << state.doraCount;
  os << " livingWall: [" << '\n';
  for (const auto& piece : state.livingWall) {
    os << piece.toStr() << ", ";
  }
  os << "]" << '\n';
  os << " livingWallIndex: " << state.livingWallIndex;
  os << " deadWall: [" << '\n';
  for (const auto& piece : state.deadWall) {
    os << piece.toStr() << ", ";
  }
  os << "]" << '\n';
  os << " DeadWallIndex: " << state.deadWallIndex;
  os << "}" << '\n';
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
  for (int i = 0; i < hand.meld_count; ++i) {
    os << "{ type: " << hand.melds[i].typeToStr();
    os << ", start: " << hand.melds[i].start.toStr() << "}";
    os << ", ";
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
