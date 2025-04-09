#include "angrydiscardobot.h"

#include "event.h"
#include "piecetype.h"
#include "winds.h"

auto AngryDiscardoBot::Name() -> std::string {
  return "AngryDiscardoBot";
}

auto AngryDiscardoBot::GameStart(int /*playerID*/) -> void {}

auto AngryDiscardoBot::RoundStart(std::vector<mahjong::Piece> hand,
                                  mahjong::Wind /*seatWind*/,
                                  mahjong::Wind /*prevalentWind*/) -> void {
  hand_ = hand;
  lastEvent_.type = mahjong::Event::kDiscard;
}

auto AngryDiscardoBot::ReceiveEvent(mahjong::Event e) -> void {
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      lastEvent_ = e;
    }
  } else if (e.type == mahjong::Event::kDiscard) {
    hand_.emplace_back(e.piece);
  }
}

auto AngryDiscardoBot::RetrieveDecision() -> mahjong::Event {
  if (lastEvent_.type == mahjong::Event::kDiscard) {
    lastEvent_.piece = hand_[n_].toUint8_t();
    n_ = (n_ + 1) % hand_.size();
  }
  mahjong::Event e = lastEvent_;
  lastEvent_.type =
      mahjong::Event::kDiscard;  // lowest """priority""" event type
  return e;
}
