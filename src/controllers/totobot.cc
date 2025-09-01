#include "controllers/totobot.h"

#include <vector>

#include "controllers/controllermanager.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {
REGISTER_PLAYER_CONTROLLER(TotoBot);

void TotoBot::GameStart(int /*playerID*/) {}

void TotoBot::RoundStart(std::vector<Piece> hand, Wind /*seatWind*/,
                         Wind /*prevalentWind*/) {
  hand_ = hand;
  lastEvent_.type = Event::kDiscard;
}

void TotoBot::ReceiveEvent(Event e) {
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      lastEvent_ = e;
    }
  } else if (e.type == Event::kDiscard) {
    hand_.emplace_back(e.piece);
  }
}

Event TotoBot::RetrieveDecision() {
  if (lastEvent_.type != Event::kDiscard) {
    lastEvent_.type = Event::kDecline;
  }
  Event e = lastEvent_;
  lastEvent_.type = Event::kDiscard;  // lowest """priority""" event type
  return e;
}
}  // namespace mahjong
