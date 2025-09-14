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
#include "types/hand.h"
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
uint8_t CountPieces(const Hand& hand, Piece p) {
  return std::ranges::count(hand.live_range(), p);
}

// Remove an instance of piece p from given players hand
uint8_t RemovePieces(Hand& hand, Piece p, uint8_t count) {
  uint8_t removed = 0;
  count = std::min(CountPieces(hand, p), count);
  const auto match_piece = [&](Piece _p) {
    if (count > removed && p == _p) {
      removed++;
      return true;
    }
    return false;
  };
  std::ranges::remove_if(hand.live, match_piece);
  hand.live_count -= removed;
  return removed;
}

// Discard an instance of piece p from given players hand
void DiscardPiece(Hand& hand, Piece p) {
  RemovePieces(hand, p, /*count=*/1);
  hand.discards[hand.discards_count++] = p;
}

Piece AskForDiscard(const GameState& state) {
  state.controllers[state.currentPlayer]->ReceiveEvent(Event{
      .type = Event::kDiscard,        // type
      .player = state.currentPlayer,  // player
      .piece = 0,                     // piece
      .decision = true,               // decision
  });

  return Piece(GetValidDecisionOrThrow(state, state.hands[state.currentPlayer],
                                       /*inHand=*/true)
                   .piece);
}

Event GetValidDecisionOrThrow(const GameState& state, const Hand& hand,
                              bool inHand) {
  Event decision;
  bool valid = false;
  int i = 0;
  while (!valid) {
    if (i > 100) {
      Event replacement_decision = decision;
      replacement_decision.type = inHand ? Event::kDiscard : Event::kDecline;
      if (inHand) {
        replacement_decision.piece = hand.live_range().back().toUint8_t();
      }
      if (ValidateDecision(state, hand, replacement_decision, inHand)) {
        return replacement_decision;
      }
      std::cerr
          << "WARNING: Player Controller sent invalid event too many times."
          << '\n';
      std::cerr << "Decision.type: " << decision.type << " Decision.piece "
                << decision.piece << " player: " << hand.id
                << " inHand: " << (inHand ? "true" : "false") << '\n';
      std::cerr << "ERROR: was not able to recover from invalid event." << '\n';
      throw 0xBAD22222;
    }
    i++;
    decision = state.controllers[hand.id]->RetrieveDecision();
    valid = ValidateDecision(state, hand, decision, inHand);
  }
  return decision;
}

bool ValidateDecision(const GameState& state, const Hand& hand, Event decision,
                      bool inHand) {
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
      return CanRon(state, hand);
    case Event::kKan:
      return CanKan(state, hand);
    case Event::kPon:
      return CanPon(state, hand);
    case Event::kChi:
      return CanChi(state, hand);
    case Event::kTsumo:
      return CanTsumo(state, hand);
    case Event::kConcealedKan:
      return CanConcealedKan(state, hand);
    case Event::kConvertedKan:
      return CanConvertedKan(state, hand);
    case Event::kRiichi:
      return CanRiichi(state, hand);
    case Event::kDiscard:
      return CountPieces(hand, Piece(decision.piece)) > 0;
    case Event::kDecline:
      return true;
    default:
      return false;
  }
}

}  // namespace mahjong
