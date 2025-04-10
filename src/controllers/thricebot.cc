#include "thricebot.h"

#include <cstddef>

#include "types/event.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/winds.h"

std::string ThriceBot::Name() {
  return "ThriceBot";
}

void ThriceBot::GameStart(int id) {
  pid_ = id;
}

const float kHonorWeight = 1.5;
const float kNormalWeight = 1;

void ThriceBot::RoundStart(std::vector<mahjong::Piece> _hand, mahjong::Wind s,
                           mahjong::Wind p) {
  for (mahjong::Piece m : _hand) {
    HandTile h;
    h.piece = m;
    if (m.isHonor()) {
      h.weight = kHonorWeight;
    } else {
      h.weight = kNormalWeight;
    }
    hand_.push_back(h);
  }
  assignweights();
  lastEvent_.type = mahjong::Event::kDiscard;
  swind_ = s;
  pwind_ = p;
}

void ThriceBot::assignweights() {
  for (size_t i = 0; i < hand_.size(); i++) {
    for (size_t j = i; j < hand_.size(); j++) {
      if (j != i) {
        if (static_cast<int>(hand_.at(i).weight) == 2 &&
            (discarded_.at(hand_.at(i).piece.toUint8_t()) == 2 ||
             discarded_.at(hand_.at(i).piece.toUint8_t()) == 2)) {
          hand_.at(i).weight -= kNormalWeight;
        } else if (hand_.at(i).piece.getSuit() == hand_[j].piece.getSuit() &&
                   hand_.at(i).piece.getPieceNum() ==
                       hand_[j].piece.getPieceNum()) {
          hand_.at(i).weight += kNormalWeight;
          hand_[j].weight += kNormalWeight;
        }
      }
    }
  }
}

HandTile ThriceBot::assignTileWeight(HandTile h1) {
  HandTile h;
  h.piece = h1.piece;
  h.weight = h1.weight;
  for (auto& i : hand_) {
    if (i.piece.getSuit() == h.piece.getSuit() &&
        i.piece.getPieceNum() == h.piece.getPieceNum()) {
      i.weight += kNormalWeight;
      h.weight += kNormalWeight;
    }
  }
  return h;
}

void ThriceBot::checkDiscard() {
  for (size_t i = 0; i < hand_.size(); i++) {
    for (size_t j = i; j < hand_.size(); j++) {
      if (j != i) {
        if (static_cast<int>(hand_.at(i).weight) == 2 &&
            (discarded_.at(hand_.at(i).piece.toUint8_t()) == 2 ||
             discarded_.at(hand_.at(i).piece.toUint8_t()) == 3)) {
          hand_.at(i).weight -= kNormalWeight;
        }
      }
    }
  }
}

void ThriceBot::ReceiveEvent(mahjong::Event e) {
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      lastEvent_ = e;
    }

    if (e.type == mahjong::Event::kDiscard && e.player == pid_) {
      HandTile h;
      h.piece = mahjong::Piece(e.piece);
      if (h.piece.isHonor()) {
        h.weight = kHonorWeight;
      } else {
        h.weight = kNormalWeight;
      }
      h = assignTileWeight(h);
      hand_.push_back(h);
      checkDiscard();
    }

  } else if (e.type == mahjong::Event::kDiscard) {
    discarded_.at(mahjong::Piece(e.piece).toUint8_t())++;
  }
}

mahjong::Event ThriceBot::RetrieveDecision() {
  if (lastEvent_.type == mahjong::Event::kDiscard) {
    lastEvent_.piece = popDiscard().toUint8_t();
  } else if (lastEvent_.type == mahjong::Event::kPon) {
    auto p = mahjong::Piece(lastEvent_.piece);
    if (!checkTile(p)) {
      lastEvent_.type = mahjong::Event::kDecline;
    }
  } else if (lastEvent_.type == mahjong::Event::kChi) {
    auto p = mahjong::Piece(lastEvent_.piece);
    if (!checkTile(p)) {
      lastEvent_.type = mahjong::Event::kDecline;
    }
  } else if (lastEvent_.type == mahjong::Event::kRiichi) {
    auto p = mahjong::Piece(lastEvent_.piece);
    if (discarded_.at(p.toUint8_t()) == 3) {
      lastEvent_.type = mahjong::Event::kDecline;
    }
  }
  mahjong::Event e = lastEvent_;
  lastEvent_.type =
      mahjong::Event::kDiscard;  // lowest """priority""" event type
  return e;
}

bool ThriceBot::checkTile(mahjong::Piece p) {
  int j = 0;
  for (auto& i : hand_) {
    if (i.piece.getSuit() == p.getSuit() &&
        i.piece.getPieceNum() == p.getPieceNum()) {
      j++;
    }
  }
  return j == 2 || j == 3;
}

mahjong::Piece ThriceBot::popDiscard() {
  if (hand_.empty()) {
    return mahjong::Piece(mahjong::kError);
  }
  int index_of_lowest = 0;
  for (size_t i = 0; i < hand_.size(); i++) {
    if (hand_.at(i).weight < hand_[index_of_lowest].weight) {
      index_of_lowest = i;
    }
  }
  mahjong::Piece p = hand_[index_of_lowest].piece;
  hand_.erase(hand_.begin() + index_of_lowest);
  discarded_.at(p.toUint8_t())++;
  return p;
}
