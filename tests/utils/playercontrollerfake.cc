#include "playercontrollerfake.h"
#include "controllers/controllermanager.h"
#include "types/event.h"

namespace mahjong {
REGISTER_PLAYER_CONTROLLER(PlayerControllerFake);

void PlayerControllerFake::ReceiveEvent(Event e) {
  events.push_back(e);
}

void PlayerControllerFake::AddEvents(std::vector<Event> e) {
  events.insert(events.end(), e.begin(), e.end());
}

std::vector<Event> PlayerControllerFake::GetEvents() {
  std::vector<Event> e;
  std::swap(events, e);
  return e;
}

Event PlayerControllerFake::RetrieveDecision() {
  if (queue.empty()) {
    throw "Not Enough Events";
  }
  Event e = queue.back();
  queue.pop_back();
  return e;
}

}  // namespace mahjong
