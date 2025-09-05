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
#include "types/hand.h"
#include "types/meld.h"
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
  os << "hands: " << '\n';
  for (size_t i = 0; i < state.players.size(); i++) {
    os << "hand[" << i << "]: {" << state.players.at(i) << "}" << '\n';
  }
  os << "controllers: " << '\n';
  for (size_t i = 0; i < state.controllers.size(); i++) {
    os << "player[" << i << "]: ";
    os << "{ controller: "
       << ((state.controllers[i] != nullptr) ? state.controllers[i]->Name()
                                             : "NULLPTR");
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

std::ostream& operator<<(std::ostream& os, const mahjong::Player& player) {
  os << "{ open: " << player.open;
  os << " riichi: " << player.riichi;
  os << " riichiPieceDiscard: " << player.riichiPieceDiscard;
  os << " riichiRound: " << player.riichiRound;
  os << " live: [" << '\n';
  os << "hasRonned: " << '\n';
  os << "hasRonned: {" << player.hasRonned << "}" << '\n';
  os << " points: " << player.points;
  for (const auto& piece : player.live_range()) {
    os << piece.toStr() << ", ";
  }
  os << "]" << '\n';
  os << "melds: [" << '\n';
  for (const auto& meld : player.melds_range()) {
    os << "{ type: " << meld.typeToStr();
    os << ", start: " << meld.start.toStr() << "}";
    os << ", ";
  }
  os << "]" << '\n';
  os << "discards: [" << '\n';
  for (const auto& piece : player.discards) {
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
