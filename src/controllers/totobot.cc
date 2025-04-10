#include "totobot.h"

#include "event.h"
#include "piecetype.h"
#include "winds.h"

std::string TotoBot::Name() {
  return "TotoBot";
}

void TotoBot::GameStart(int /*playerID*/) {}

void TotoBot::RoundStart(std::vector<mahjong::Piece> hand,
                         mahjong::Wind /*seatWind*/,
                         mahjong::Wind /*prevalentWind*/) {
  hand_ = hand;
  lastEvent_.type = mahjong::Event::kDiscard;
}

void TotoBot::ReceiveEvent(mahjong::Event e) {
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      lastEvent_ = e;
    }
  } else if (e.type == mahjong::Event::kDiscard) {
    hand_.emplace_back(e.piece);
  }
}

mahjong::Event TotoBot::RetrieveDecision() {
  if (lastEvent_.type != mahjong::Event::kDiscard) {
    lastEvent_.type = mahjong::Event::kDecline;
  }
  mahjong::Event e = lastEvent_;
  lastEvent_.type =
      mahjong::Event::kDiscard;  // lowest """priority""" event type
  return e;
}
