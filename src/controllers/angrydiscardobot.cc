#include "angrydiscardobot.h"

#include "controllermanager.h"
#include "types/event.h"
#include "types/piecetype.h"

namespace mahjong {
REGISTER_PLAYER_CONTROLLER(AngryDiscardoBot);

void AngryDiscardoBot::ReceiveEvent(Event e) {
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      lastEvent_ = e;
    }
  } else if (e.type == Event::kDiscard) {
    hand_.emplace_back(e.piece);
  }
}

Event AngryDiscardoBot::RetrieveDecision() {
  if (lastEvent_.type == Event::kDiscard) {
    lastEvent_.piece = hand_[n_].toUint8_t();
    n_ = (n_ + 1) % hand_.size();
  }
  Event e = lastEvent_;
  lastEvent_.type = Event::kDiscard;  // lowest """priority""" event type
  return e;
}
}  // namespace mahjong
