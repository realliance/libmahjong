#include "statefunctions/stateutilities.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ranges>

#include "controllers/playercontroller.h"
#include "statefunctions/decisionfunction.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {

Wind GetSeat(int round, int player) {
  return static_cast<Wind>((player + 3 * (round % 4)) % 4);
}

// Push Event to Player Queue
void AlertPlayers(const GameState& state, Event e) {
  e.decision = false;
  for (const auto& controller : state.controllers) {
    controller->ReceiveEvent(e);
  }
}

// Count number of piece p that are in given players hands
uint8_t CountPieces(const Player& player, Piece p) {
  return std::ranges::count(player.live_range(), p);
}

// Remove an instance of piece p from given players hand
uint8_t RemovePieces(Player& player, Piece p, uint8_t count) {
  uint8_t removed = 0;
  count = std::min(CountPieces(player, p), count);
  const auto match_piece = [&](Piece _p) {
    if (count > removed && p == _p) {
      removed++;
      return true;
    }
    return false;
  };
  std::ranges::remove_if(player.live, match_piece);
  player.live_count -= removed;
  return removed;
}

// Discard an instance of piece p from given players hand
void DiscardPiece(Player& player, Piece p) {
  RemovePieces(player, p, /*count=*/1);
  player.discards[player.discards_count++] = p;
}

Piece AskForDiscard(const GameState& state) {
  state.controllers.at(state.currentPlayer)
      ->ReceiveEvent(Event{
          .type = Event::kDiscard,        // type
          .player = state.currentPlayer,  // player
          .piece = 0,                     // piece
          .decision = true,               // decision
      });

  return Piece(
      GetValidDecisionOrThrow(state, state.players[state.currentPlayer], /*inPlayer=*/true)
          .piece);
}

Event GetValidDecisionOrThrow(const GameState& state, const Player& player,
                              bool inPlayer) {
  Event decision;
  bool valid = false;
  int i = 0;
  while (!valid) {
    if (i > 100) {
      Event replacement_decision = decision;
      replacement_decision.type = inPlayer ? Event::kDiscard : Event::kDecline;
      if (inPlayer) {
        replacement_decision.piece = player.live_range().back().toUint8_t();
      }
      if (ValidateDecision(state, player, replacement_decision, inPlayer)) {
        return replacement_decision;
      }
      std::cerr
          << "WARNING: Player Controller sent invalid event too many times."
          << '\n';
      std::cerr << "Decision.type: " << decision.type << " Decision.piece "
                << decision.piece << " player: " << player.id
                << " inPlayer: " << (inPlayer ? "true" : "false") << '\n';
      std::cerr << "ERROR: was not able to recover from invalid event." << '\n';
      throw 0xBAD22222;
    }
    i++;
    decision = state.controllers.at(player.id)->RetrieveDecision();
    valid = ValidateDecision(state, player, decision, inPlayer);
  }
  return decision;
}

bool ValidateDecision(const GameState& state, const Player& player,
                      Event decision, bool inPlayer) {
  if (decision.type > Event::kDiscard) {
    return false;
  }
  if (decision.type > Event::kDecline && !inPlayer) {
    return false;
  }
  if (decision.type < Event::kTsumo && inPlayer) {
    return false;
  }
  switch (decision.type) {
    case Event::kRon:
      return CanRon(state, player);
    case Event::kKan:
      return CanKan(state, player);
    case Event::kPon:
      return CanPon(state, player);
    case Event::kChi:
      return CanChi(state, player);
    case Event::kTsumo:
      return CanTsumo(state, player);
    case Event::kConcealedKan:
      return CanConcealedKan(state, player);
    case Event::kConvertedKan:
      return CanConvertedKan(state, player);
    case Event::kRiichi:
      return CanRiichi(state, player);
    case Event::kDiscard:
      return CountPieces(player, Piece(decision.piece)) > 0;
    case Event::kDecline:
      return true;
    default:
      return false;
  }
}

}  // namespace mahjong
