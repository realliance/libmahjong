#include "angrydiscardobot.h"

#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

std::string AngryDiscardoBot::Name() {
  return "AngryDiscardoBot";
}

void AngryDiscardoBot::GameStart(int /*playerID*/) {}

void AngryDiscardoBot::RoundStart(std::vector<mahjong::Piece> hand,
                                  mahjong::Wind /*seatWind*/,
                                  mahjong::Wind /*prevalentWind*/) {
  hand_ = hand;
  lastEvent_.type = mahjong::Event::kDiscard;
}

void AngryDiscardoBot::ReceiveEvent(mahjong::Event e) {
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      lastEvent_ = e;
    }
  } else if (e.type == mahjong::Event::kDiscard) {
    hand_.emplace_back(e.piece);
  }
}

mahjong::Event AngryDiscardoBot::RetrieveDecision() {
  if (lastEvent_.type == mahjong::Event::kDiscard) {
    lastEvent_.piece = hand_[n_].toUint8_t();
    n_ = (n_ + 1) % hand_.size();
  }
  mahjong::Event e = lastEvent_;
  lastEvent_.type =
      mahjong::Event::kDiscard;  // lowest """priority""" event type
  return e;
}
