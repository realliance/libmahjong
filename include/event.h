#pragma once
#include <cstdint>
#include <iostream>
#include <string>

namespace Mahjong {

struct Event {
  enum Type {
    // out of hand events
    Ron,
    Kan,
    Pon,
    Chi,
    Decline,
    // in hand events
    Tsumo,
    ConcealedKan,
    ConvertedKan,
    Riichi,
    Discard,
    // other game events
    Dora,
    PointDiff,
    ExhaustiveDraw,  //not used should be tho
    End
  };
  Type type = {};
  int player = {};
  int16_t piece = {};
  bool decision = {};
};

const Event END_EVENT = {Event::End, -1, 0, false};
const Event DECLINE_EVENT = {Event::Decline, -1, 0, false};

inline auto EventTypeToStr(Event::Type s) -> std::string {
  switch (s) {
    case Event::Ron:
      return "Ron";
    case Event::Kan:
      return "Kan";
    case Event::Pon:
      return "Pon";
    case Event::Chi:
      return "Chi";
    case Event::Tsumo:
      return "Tsumo";
    case Event::ConcealedKan:
      return "ConcealedKan";
    case Event::ConvertedKan:
      return "ConvertedKan";
    case Event::Riichi:
      return "Riichi";
    case Event::Discard:
      return "Discard";
    case Event::Decline:
      return "Decline";
    case Event::Dora:
      return "Dora";
    case Event::PointDiff:
      return "PointDiff";
    case Event::ExhaustiveDraw:
      return "ExhaustiveDraw";
    case Event::End:
      return "End";
    default:
      return "InvalidState";
  }
}

}  // namespace Mahjong

auto operator<<(std::ostream& os, const Mahjong::Event& e) -> std::ostream&;
