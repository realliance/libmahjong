#include "playercontrollerfake.h"
#include "controllers/controllermanager.h"
#include "types/event.h"

namespace mahjong {
REGISTER_PLAYER_CONTROLLER(PlayerControllerFake);

void PlayerControllerFake::ReceiveEvent(Event e) {
  events_.push_back(e);
}

void PlayerControllerFake::AddEvents(std::vector<Event> e) {
  events_.insert(events_.end(), e.begin(), e.end());
}

std::vector<Event> PlayerControllerFake::GetEvents() {
  std::vector<Event> e;
  std::swap(events_, e);
  return e;
}

Event PlayerControllerFake::RetrieveDecision() {
  if (queue_.empty()) {
    throw "Not Enough events_";
  }
  Event e = queue_.back();
  queue_.pop_back();
  return e;
}

}  // namespace mahjong
