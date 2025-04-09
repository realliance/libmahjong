#include "stateutilities.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include "decisionfunction.h"
#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "player.h"
#include "playercontroller.h"

namespace mahjong {

auto GetSeat(int round, int player) -> Wind {
  return static_cast<Wind>((player + 3 * (round % 4)) % 4);
}

// Push Event to Player Queue
auto AlertPlayers(const GameState& state, Event e) -> void {
  e.decision = false;
  for (const auto& player : state.players) {
    player.controller->ReceiveEvent(e);
  }
}

// Count number of piece p that are in given players hands
auto CountPieces(const GameState& state, int player, Piece p) -> uint8_t {
  return std::count(state.hands.at(player).live.begin(),
                    state.hands.at(player).live.end(), p);
}

// Remove an instance of piece p from given players hand
auto RemovePieces(GameState& state, int player, Piece p, uint8_t count)
    -> uint8_t {
  count = std::min(CountPieces(state, player, p), count);
  uint8_t removed = 0;
  state.hands.at(player).live.erase(
      std::remove_if(state.hands.at(player).live.begin(),
                     state.hands.at(player).live.end(),
                     [&](Piece _p) {
                       if (count > removed && p == _p) {
                         removed++;
                         return true;
                       }
                       return false;
                     }),
      state.hands.at(player).live.end());
  return removed;
}

// Discard an instance of piece p from given players hand
auto DiscardPiece(GameState& state, int player, Piece p) -> void {
  RemovePieces(state, player, p, /*count=*/1);
  state.hands.at(player).discards.push_back(p);
}

auto AskForDiscard(const GameState& state) -> Piece {
  state.players.at(state.currentPlayer)
      .controller->ReceiveEvent(Event{
          .type = Event::kDiscard,        // type
          .player = state.currentPlayer,  // player
          .piece = 0,                     // piece
          .decision = true,               // decision
      });

  return Piece(
      GetValidDecisionOrThrow(state, state.currentPlayer, /*inHand=*/true)
          .piece);
}

auto GetValidDecisionOrThrow(const GameState& state, int player, bool inHand)
    -> Event {
  Event decision;
  bool valid = false;
  int i = 0;
  while (!valid) {
    if (i > 100) {
      Event replacement_decision = decision;
      replacement_decision.type = inHand ? Event::kDiscard : Event::kDecline;
      if (inHand) {
        replacement_decision.piece = static_cast<int16_t>(
            state.hands.at(player).live.back().toUint8_t());
      }
      if (ValidateDecision(state, player, replacement_decision, inHand)) {
        return replacement_decision;
      }
      std::cerr
          << "WARNING: Player Controller sent invalid event too many times."
          << '\n';
      std::cerr << "Decision.type: " << decision.type << " Decision.piece "
                << decision.piece << " player: " << player
                << " inHand: " << (inHand ? "true" : "false") << '\n';
      std::cerr << "ERROR: was not able to recover from invalid event." << '\n';
      throw 0xBAD22222;
    }
    i++;
    if (state.halt) {
      throw 0xFACEFEED;
    }
    decision = state.players.at(player).controller->RetrieveDecision();
    valid = ValidateDecision(state, player, decision, inHand);
  }
  return decision;
}

auto ValidateDecision(const GameState& state, int player, Event decision,
                      bool inHand) -> bool {
  if (decision.type > Event::kDiscard) {
    return false;
  }
  if (decision.type > Event::kDecline && !inHand) {
    return false;
  }
  if (decision.type < Event::kTsumo && inHand) {
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
      return CanTsumo(state);
    case Event::kConcealedKan:
      return CanConcealedKan(state);
    case Event::kConvertedKan:
      return CanConvertedKan(state);
    case Event::kRiichi:
      return CanRiichi(state);
    case Event::kDiscard:
      return CountPieces(state, player, Piece(decision.piece)) > 0;
    case Event::kDecline:
      return true;
    default:
      return false;
  }
}

}  // namespace mahjong
