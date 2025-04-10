#pragma once
#include <cstdint>
#include <iostream>
#include <string>

namespace mahjong {

struct Event {
  enum Type {
    // out of hand events
    kRon,
    kKan,
    kPon,
    kChi,
    kDecline,
    // in hand events
    kTsumo,
    kConcealedKan,
    kConvertedKan,
    kRiichi,
    kDiscard,
    // other game events
    kDora,
    kPointDiff,
    kExhaustiveDraw,  // not used should be tho
    kEnd
  };
  Type type = {};
  int player = {};
  int16_t piece = {};
  bool decision = {};
};

const Event kEndEvent = {.type = Event::kEnd,
                         .player = -1,
                         .piece = 0,
                         .decision = false};
const Event kDeclineEvent = {.type = Event::kDecline,
                             .player = -1,
                             .piece = 0,
                             .decision = false};

inline std::string EventTypeToStr(Event::Type s) {
  switch (s) {
    case Event::kRon:
      return "Ron";
    case Event::kKan:
      return "Kan";
    case Event::kPon:
      return "Pon";
    case Event::kChi:
      return "Chi";
    case Event::kTsumo:
      return "Tsumo";
    case Event::kConcealedKan:
      return "ConcealedKan";
    case Event::kConvertedKan:
      return "ConvertedKan";
    case Event::kRiichi:
      return "Riichi";
    case Event::kDiscard:
      return "Discard";
    case Event::kDecline:
      return "Decline";
    case Event::kDora:
      return "Dora";
    case Event::kPointDiff:
      return "PointDiff";
    case Event::kExhaustiveDraw:
      return "ExhaustiveDraw";
    case Event::kEnd:
      return "End";
    default:
      return "InvalidState";
  }
}

}  // namespace mahjong

std::ostream& operator<<(std::ostream& os, const mahjong::Event& e);
