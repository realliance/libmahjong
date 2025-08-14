#include "playercontrollerfake.h"

#include <utility>

#include "controllers/controllermanager.h"
#include "types/event.h"

namespace mahjong {
REGISTER_PLAYER_CONTROLLER(PlayerControllerFake);

Event PlayerControllerFake::DefaultDecisionCallback(const Event& e) {
  Event decision = e;
  // For discard events, keep them as is
  // For all other events (like calls), decline them
  if (e.type != Event::kDiscard) {
    decision.type = Event::kDecline;
    decision.decision = false;
  }
  return decision;
}

PlayerControllerFake::PlayerControllerFake()
    : decisionCallback_(DefaultDecisionCallback), lastEvent_{} {}

PlayerControllerFake::PlayerControllerFake(DecisionCallback callback)
    : decisionCallback_(std::move(callback)), lastEvent_{} {}

void PlayerControllerFake::ReceiveEvent(Event e) {
  lastEvent_ = e;
}

Event PlayerControllerFake::RetrieveDecision() {
  if (lastEvent_.decision) {
    // Call the callback with the last seen event
    if (decisionCallback_) {
      Event decision = decisionCallback_(lastEvent_);
      decision.player = lastEvent_.player;
      return decision;
    }
    // Or default decision
    Event decision = DefaultDecisionCallback(lastEvent_);
    decision.player = lastEvent_.player;
    return decision;
  }

  // No decision event stored, just decline
  return Event{
      .type = Event::kDecline, .player = 0, .piece = 0, .decision = false};
}

}  // namespace mahjong
