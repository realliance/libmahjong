#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

#include "decisionfunction.h"
#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "piecetype.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"
#include "stateutilities.h"

auto mahjong::PlayerHand(GameState& state) -> GameState& {
  using DecisionFunction = auto (*)(const mahjong::GameState& state)->bool;

  struct PossibleDecision {
    Event::Type type;
    DecisionFunction func;
  };

  std::vector<PossibleDecision> decisions = {
      {.type = Event::kTsumo, .func = CanTsumo},
      {.type = Event::kConcealedKan, .func = CanConcealedKan},
      {.type = Event::kConvertedKan, .func = CanConvertedKan},
      {.type = Event::kRiichi, .func = CanRiichi},
      {.type = Event::kDiscard, .func = [](const GameState& state) {
         return !state.hands.at(state.currentPlayer).riichi;
       }}};

  bool decision_asked = false;
  for (const auto& [decision, decisionIsPossible] : decisions) {
    if (decisionIsPossible(state)) {
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

  return state;
}
