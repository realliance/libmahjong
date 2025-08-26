#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "statefunctions/decisionfunction.h"
#include "statefunctions/router.h"
#include "statefunctions/stateutilities.h"
#include "types/event.h"
#include "types/gamestate.h"
#include "types/piecetype.h"
#include "types/statefunction.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> PlayerHand(std::unique_ptr<GameState> state) {
  const std::vector<PossibleDecision> decisions = {
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
    if (decisionIsPossible(*state, state->currentPlayer)) {
      decision_asked = true;
      state->players.at(state->currentPlayer)
          ->ReceiveEvent(Event{
              .type = decision,                // type
              .player = state->currentPlayer,  // player
              .piece = static_cast<int16_t>(
                  state->pendingPiece.toUint8_t()),  // piece
              .decision = true,                      // decision
          });
    }
  }

  Event decision;
  if (!decision_asked) {
    decision.type = Event::kDiscard;
    decision.piece = static_cast<uint16_t>(state->pendingPiece.toUint8_t());
    decision.player = state->currentPlayer;
    decision.decision = true;
    state->nextState = StateFunctionType::kDiscard;
  } else {
    decision =
        GetValidDecisionOrThrow(*state, state->currentPlayer, /*inHand=*/true);
  }

  // note riichi handling is a lil borked on the player agency side
  // checkout riichi.cpp for more info
  if (decision.type == Event::kDiscard) {
    state->pendingPiece = Piece(decision.piece);
  }

  switch (decision.type) {
    case Event::kTsumo:
      state->nextState = StateFunctionType::kTsumo;
      break;
    case Event::kConcealedKan:
      state->nextState = StateFunctionType::kConcealedKan;
      break;
    case Event::kConvertedKan:
      state->nextState = StateFunctionType::kConvertedKan;
      break;
    case Event::kRiichi:
      state->nextState = StateFunctionType::kRiichi;
      break;
    case Event::kDiscard:
      state->nextState = StateFunctionType::kDiscard;
      break;
    default:
      std::cerr << "Invalid Decision Type in playerhand: " << decision.type
                << '\n';
      state->nextState = StateFunctionType::kError;
      break;
  }

  return state;
}
}  // namespace

REGISTER_ROUTE(PlayerHand, StateFunctionType::kPlayerHand);
}  // namespace mahjong
