#include "playercontrollerfake.h"

#include <utility>
#include <vector>

#include "controllers/controllermanager.h"
#include "types/event.h"

namespace mahjong {
REGISTER_PLAYER_CONTROLLER(PlayerControllerFake);

void PlayerControllerFake::ReceiveEvent(Event e) {
  events_.push_back(e);
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      // Use pre-loaded decision if available, otherwise use the received event
      if (!queue_.empty()) {
        Event preloaded_decision = queue_.front();
        queue_.erase(queue_.begin());
        // Copy relevant info from received event
        preloaded_decision.player = e.player;
        lastEvent_ = preloaded_decision;
      } else {
        lastEvent_ = e;
      }
    }
  }
}

void PlayerControllerFake::AddEvents(std::vector<Event> e) {
  queue_.insert(queue_.end(), e.begin(), e.end());
}

std::vector<Event> PlayerControllerFake::GetEvents() {
  std::vector<Event> e;
  std::swap(events_, e);
  return e;
}

Event PlayerControllerFake::RetrieveDecision() {
  if (lastEvent_.type != Event::kDiscard) {
    lastEvent_.type = Event::kDecline;
  }
  Event e = lastEvent_;
  lastEvent_.type = Event::kDiscard;  // Reset for next decision
  return e;
}

}  // namespace mahjong
