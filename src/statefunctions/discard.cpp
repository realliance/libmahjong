#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

#include "decisionfunction.h"
#include "event.h"
#include "gamestate.h"
#include "piecetype.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"
#include "stateutilities.h"
#include "walls.h"

auto Mahjong::Discard(GameState& state) -> GameState& {
  AlertPlayers(state, Event{
                        Event::Discard,                                        // type
                        state.currentPlayer,                                   // player
                        static_cast<int16_t>(state.pendingPiece.toUint8_t()),  // piece
                        false,                                                 // decision
                      });
  DiscardPiece(state, state.currentPlayer, state.pendingPiece);

  using DecisionFunction = auto (*)(const Mahjong::GameState& state, int player)->bool;

  struct possibleDecision {
    Event::Type type;
    DecisionFunction func;
  };

  std::vector<possibleDecision> decisions = {
    {Event::Chi, CanChi},
    {Event::Pon, CanPon},
    {Event::Kan, CanKan},
    {Event::Ron, CanRon}};

  std::array<bool, 4> needDecision = {false, false, false, false};
  for (int player = 0; player < 4; player++) {
    if (player == state.currentPlayer) {
      continue;
    }
    for (const auto& [decision, decisionIsPossible] : decisions) {
      if (decisionIsPossible(state, player)) {
        needDecision.at(player) = true;
        state.players.at(player).controller->ReceiveEvent(
          Event{
            decision,                                              // type
            state.currentPlayer,                                   // player
            static_cast<int16_t>(state.pendingPiece.toUint8_t()),  // piece
            true,                                                  // decision
          });
      }
    }
  }

  Event decision = DECLINE_EVENT;
  for (int i = 0; i < 4; i++) {
    if (needDecision.at(i)) {
      Event tempDecision = GetValidDecisionOrThrow(state, i, /*inHand=*/false);
      if (tempDecision.type < decision.type) {  // lower is higher priority
        tempDecision.player = i;
        tempDecision.piece = static_cast<int16_t>(state.pendingPiece.toUint8_t());
        decision = tempDecision;
      }
      if (tempDecision.type == Event::Ron) {
        state.hasRonned.at(i) = true;
      }
    }
  }

  if (decision.type == Event::Decline && state.walls.GetRemainingPieces() == 0) {
    state.nextState = Exhaust;
    return state;
  }

  if (decision.type != Event::Decline) {
    state.lastCaller = decision.player;
  }

  switch (decision.type) {
    case Event::Decline:
      state.nextState = Draw;
      break;
    case Event::Ron:
      state.nextState = Ron;
      break;
    case Event::Chi:
      state.nextState = Chi;
      break;
    case Event::Pon:
      state.nextState = Pon;
      break;
    case Event::Kan:
      state.nextState = Kan;
      break;
    default:
      std::cerr << "Invalid Decision Type in Discard: " << decision.type << std::endl;
      state.nextState = Error;
      break;
  }

  return state;
}
