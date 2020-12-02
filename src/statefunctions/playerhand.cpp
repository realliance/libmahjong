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

auto Mahjong::PlayerHand(GameState& state) -> GameState& {
  using DecisionFunction = auto (*)(const Mahjong::GameState& state)->bool;

  struct possibleDecision {
    Event::Type type;
    DecisionFunction func;
  };

  std::vector<possibleDecision> decisions = {
    {Event::Tsumo, CanTsumo},
    {Event::ConcealedKan, CanConcealedKan},
    {Event::ConvertedKan, CanConvertedKan},
    {Event::Riichi, CanRiichi},
    {Event::Discard, [](const GameState& state) {
       return !state.hands.at(state.currentPlayer).riichi;
     }}};

  bool decisionAsked = false;
  for (const auto& [decision, decisionIsPossible] : decisions) {
    if (decisionIsPossible(state)) {
      decisionAsked = true;
      state.players.at(state.currentPlayer).controller->ReceiveEvent(Event{
        decision,                                              // type
        state.currentPlayer,                                   // player
        static_cast<int16_t>(state.pendingPiece.toUint8_t()),  // piece
        true,                                                  // decision
      });
    }
  }

  Event decision;
  if (!decisionAsked) {
    decision.type = Event::Discard;
    decision.piece = static_cast<uint16_t>(state.pendingPiece.toUint8_t());
    decision.player = state.currentPlayer;
    decision.decision = true;
    state.nextState = Discard;
  } else {
    decision = GetValidDecisionOrThrow(state, state.currentPlayer, /*inHand=*/true);
  }

  //note riichi handling is a lil borked on the player agency side
  //checkout riichi.cpp for more info
  if (decision.type == Event::Discard) {
    state.pendingPiece = decision.piece;
  }

  switch (decision.type) {
    case Event::Tsumo:
      state.nextState = Tsumo;
      break;
    case Event::ConcealedKan:
      state.nextState = ConcealedKan;
      break;
    case Event::ConvertedKan:
      state.nextState = ConvertedKan;
      break;
    case Event::Riichi:
      state.nextState = Riichi;
      break;
    case Event::Discard:
      state.nextState = Discard;
      break;
    default:
      std::cerr << "Invalid Decision Type in playerhand: " << decision.type << std::endl;
      state.nextState = Error;
      break;
  }

  return state;
}
