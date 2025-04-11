#include <array>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

#include "controllers/playercontroller.h"
#include "statefunctions/decisionfunction.h"
#include "statefunctions/statefunctions.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/player.h"

namespace mahjong {

GameState&& PlayerHand(GameState&& state) {
  std::vector<PossibleDecision> decisions = {
      PossibleDecision{.type = Event::kTsumo, .func = CanTsumo},
      PossibleDecision{.type = Event::kConcealedKan, .func = CanConcealedKan},
      PossibleDecision{.type = Event::kConvertedKan, .func = CanConvertedKan},
      PossibleDecision{.type = Event::kRiichi, .func = CanRiichi},
      PossibleDecision{.type = Event::kDiscard,
                       .func = [](const GameState& state, int) {
                         return !state.hands.at(state.currentPlayer).riichi;
                       }}};

  bool decision_asked = false;
  for (const auto& [decision, decisionIsPossible] : decisions) {
    if (decisionIsPossible(state, state.currentPlayer)) {
      decision_asked = true;
      state.players.at(state.currentPlayer)
          .controller->ReceiveEvent(Event{
              .type = decision,               // type
              .player = state.currentPlayer,  // player
              .piece = static_cast<int16_t>(
                  state.pendingPiece.toUint8_t()),  // piece
              .decision = true,                     // decision
          });
    }
  }

  Event decision;
  if (!decision_asked) {
    decision.type = Event::kDiscard;
    decision.piece = static_cast<uint16_t>(state.pendingPiece.toUint8_t());
    decision.player = state.currentPlayer;
    decision.decision = true;
    state.nextState = Discard;
  } else {
    decision =
        GetValidDecisionOrThrow(state, state.currentPlayer, /*inHand=*/true);
  }

  // note riichi handling is a lil borked on the player agency side
  // checkout riichi.cpp for more info
  if (decision.type == Event::kDiscard) {
    state.pendingPiece = Piece(decision.piece);
  }

  switch (decision.type) {
    case Event::kTsumo:
      state.nextState = Tsumo;
      break;
    case Event::kConcealedKan:
      state.nextState = ConcealedKan;
      break;
    case Event::kConvertedKan:
      state.nextState = ConvertedKan;
      break;
    case Event::kRiichi:
      state.nextState = Riichi;
      break;
    case Event::kDiscard:
      state.nextState = Discard;
      break;
    default:
      std::cerr << "Invalid Decision Type in playerhand: " << decision.type
                << '\n';
      state.nextState = Error;
      break;
  }

  return std::move(state);
}

}  // namespace mahjong
