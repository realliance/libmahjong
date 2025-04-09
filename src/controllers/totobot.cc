#include "totobot.h"

#include "event.h"
#include "piecetype.h"
#include "winds.h"

auto TotoBot::Name() -> std::string {
  return "TotoBot";
}

auto TotoBot::GameStart(int /*playerID*/) -> void {}

auto TotoBot::RoundStart(std::vector<mahjong::Piece> hand,
                         mahjong::Wind /*seatWind*/,
                         mahjong::Wind /*prevalentWind*/) -> void {
  hand_ = hand;
  lastEvent_.type = mahjong::Event::kDiscard;
}

auto TotoBot::ReceiveEvent(mahjong::Event e) -> void {
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      lastEvent_ = e;
    }
  } else if (e.type == mahjong::Event::kDiscard) {
    hand_.emplace_back(e.piece);
  }
}

auto TotoBot::RetrieveDecision() -> mahjong::Event {
  if (lastEvent_.type != mahjong::Event::kDiscard) {
    lastEvent_.type = mahjong::Event::kDecline;
  }
  mahjong::Event e = lastEvent_;
  lastEvent_.type =
      mahjong::Event::kDiscard;  // lowest """priority""" event type
  return e;
}
