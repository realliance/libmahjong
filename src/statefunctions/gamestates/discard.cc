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
#include "types/hand.h"
#include "types/piecetype.h"
#include "types/statefunction.h"
#include "types/walls.h"

namespace mahjong {

namespace {
std::unique_ptr<GameState> Discard(std::unique_ptr<GameState> state) {
  AlertPlayers(*state, Event{
                           .type = Event::kDiscard,         // type
                           .player = state->currentPlayer,  // player
                           .piece = static_cast<int16_t>(
                               state->pendingPiece.toUint8_t()),  // piece
                           .decision = false,                     // decision
                       });
  DiscardPiece(state->players[state->currentPlayer], state->pendingPiece);

  const std::vector<PossibleDecision> decisions = {
      {.type = Event::kChi, .func = CanChi},
      {.type = Event::kPon, .func = CanPon},
      {.type = Event::kKan, .func = CanKan},
      {.type = Event::kRon, .func = CanRon},
  };

  std::array<bool, 4> need_decision = {false, false, false, false};
  for (const Hand& player : state->players) {
    if (player.id == state->currentPlayer) {
      continue;
    }
    for (const auto& [decision, decisionIsPossible] : decisions) {
      if (decisionIsPossible(*state, player)) {
        need_decision.at(player.id) = true;
        state->controllers.at(player.id)->ReceiveEvent(Event{
            .type = decision,                // type
            .player = state->currentPlayer,  // player
            .piece =
                static_cast<int16_t>(state->pendingPiece.toUint8_t()),  // piece
            .decision = true,  // decision
        });
      }
    }
  }

  Event decision = kDeclineEvent;
  for (Hand& player : state->players) {
    if (need_decision.at(player.id)) {
      Event temp_decision =
          GetValidDecisionOrThrow(*state, player, /*inHand=*/false);
      if (temp_decision.type < decision.type) {  // lower is higher priority
        temp_decision.player = player.id;
        temp_decision.piece =
            static_cast<int16_t>(state->pendingPiece.toUint8_t());
        decision = temp_decision;
      }
      if (temp_decision.type == Event::kRon) {
        player.hasRonned = true;
      }
    }
  }

  if (decision.type == Event::kDecline &&
      Walls::GetRemainingPieces(*state) == 0) {
    state->nextState = StateFunctionType::kExhaust;
    return state;
  }

  if (decision.type != Event::kDecline) {
    state->lastCaller = decision.player;
  }

  switch (decision.type) {
    case Event::kDecline:
      state->nextState = StateFunctionType::kDraw;
      break;
    case Event::kRon:
      state->nextState = StateFunctionType::kRon;
      break;
    case Event::kChi:
      state->nextState = StateFunctionType::kChi;
      break;
    case Event::kPon:
      state->nextState = StateFunctionType::kPon;
      break;
    case Event::kKan:
      state->nextState = StateFunctionType::kKan;
      break;
    default:
      std::cerr << "Invalid Decision Type in Discard: " << decision.type
                << '\n';
      state->nextState = StateFunctionType::kError;
      break;
  }

  return state;
}
}  // namespace

REGISTER_ROUTE(Discard, StateFunctionType::kDiscard);
}  // namespace mahjong
