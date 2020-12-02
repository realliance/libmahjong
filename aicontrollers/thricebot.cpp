#include "thricebot.h"

#include <cstddef>
#include <cstdint>
#include <memory>

#include "event.h"
#include "piecetype.h"
#include "winds.h"

auto ThriceBot::Name() -> std::string {
  return "ThriceBot";
}

auto ThriceBot::GameStart(int id) -> void {
  pid = id;
}

const float HONOR_WEIGHT = 1.5;
const float NORMAL_WEIGHT = 1;

auto ThriceBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind s, Mahjong::Wind p) -> void {
  for (Mahjong::Piece m : _hand) {
    handTile h;
    h.piece = m;
    if (m.isHonor()) {
      h.weight = HONOR_WEIGHT;
    } else {
      h.weight = NORMAL_WEIGHT;
    }
    hand.push_back(h);
  }
  assignweights();
  lastEvent.type = Mahjong::Event::Discard;
  swind = s;
  pwind = p;
}

void ThriceBot::assignweights() {
  for (size_t i = 0; i < hand.size(); i++) {
    for (size_t j = i; j < hand.size(); j++) {
      if (j != i) {
        if (static_cast<int>(hand.at(i).weight) == 2 && (discarded.at(hand.at(i).piece.toUint8_t()) == 2 || discarded.at(hand.at(i).piece.toUint8_t()) == 2)) {
          hand.at(i).weight -= NORMAL_WEIGHT;
        } else if (hand.at(i).piece.getSuit() == hand[j].piece.getSuit() && hand.at(i).piece.getPieceNum() == hand[j].piece.getPieceNum()) {
          hand.at(i).weight += NORMAL_WEIGHT;
          hand[j].weight += NORMAL_WEIGHT;
        }
      }
    }
  }
}

auto ThriceBot::assignTileWeight(handTile h1) -> handTile {
  handTile h;
  h.piece = h1.piece;
  h.weight = h1.weight;
  for (auto& i : hand) {
    if (i.piece.getSuit() == h.piece.getSuit() && i.piece.getPieceNum() == h.piece.getPieceNum()) {
      i.weight += NORMAL_WEIGHT;
      h.weight += NORMAL_WEIGHT;
    }
  }
  return h;
}

void ThriceBot::checkDiscard() {
  for (size_t i = 0; i < hand.size(); i++) {
    for (size_t j = i; j < hand.size(); j++) {
      if (j != i) {
        if (static_cast<int>(hand.at(i).weight) == 2 && (discarded.at(hand.at(i).piece.toUint8_t()) == 2 || discarded.at(hand.at(i).piece.toUint8_t()) == 3)) {
          hand.at(i).weight -= NORMAL_WEIGHT;
        }
      }
    }
  }
}

auto ThriceBot::ReceiveEvent(Mahjong::Event e) -> void {
  if (e.decision) {
    if (e.type <= lastEvent.type) {
      lastEvent = e;
    }

    if (e.type == Mahjong::Event::Discard && e.player == pid) {
      handTile h;
      h.piece = Mahjong::Piece(e.piece);
      if (h.piece.isHonor()) {
        h.weight = HONOR_WEIGHT;
      } else {
        h.weight = NORMAL_WEIGHT;
      }
      h = assignTileWeight(h);
      hand.push_back(h);
      checkDiscard();
    }

  } else if (e.type == Mahjong::Event::Discard) {
    discarded.at(Mahjong::Piece(e.piece).toUint8_t())++;
  }
}

auto ThriceBot::RetrieveDecision() -> Mahjong::Event {
  if (lastEvent.type == Mahjong::Event::Discard) {
    lastEvent.piece = popDiscard().toUint8_t();
  } else if (lastEvent.type == Mahjong::Event::Pon) {
    auto p = Mahjong::Piece(lastEvent.piece);
    if (!checkTile(p)) {
      lastEvent.type = Mahjong::Event::Decline;
    }
  } else if (lastEvent.type == Mahjong::Event::Chi) {
    auto p = Mahjong::Piece(lastEvent.piece);
    if (!checkTile(p)) {
      lastEvent.type = Mahjong::Event::Decline;
    }
  } else if (lastEvent.type == Mahjong::Event::Riichi) {
    auto p = Mahjong::Piece(lastEvent.piece);
    if (discarded.at(p.toUint8_t()) == 3) {
      lastEvent.type = Mahjong::Event::Decline;
    }
  }
  Mahjong::Event e = lastEvent;
  lastEvent.type = Mahjong::Event::Discard;  // lowest """priority""" event type
  return e;
}

auto ThriceBot::checkTile(Mahjong::Piece p) -> bool {
  int j = 0;
  for (auto& i : hand) {
    if (i.piece.getSuit() == p.getSuit() && i.piece.getPieceNum() == p.getPieceNum()) {
      j++;
    }
  }
  return j == 2 || j == 3;
}

auto ThriceBot::popDiscard() -> Mahjong::Piece {
  if (hand.empty()) {
    return Mahjong::Piece(Mahjong::Piece::ERROR);
  }
  int indexOfLowest = 0;
  for (size_t i = 0; i < hand.size(); i++) {
    if (hand.at(i).weight < hand[indexOfLowest].weight) {
      indexOfLowest = i;
    }
  }
  Mahjong::Piece p = hand[indexOfLowest].piece;
  hand.erase(hand.begin() + indexOfLowest);
  discarded.at(p.toUint8_t())++;
  return p;
}
