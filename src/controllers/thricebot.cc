#include "controllers/thricebot.h"

#include <cstddef>
#include <vector>

#include "controllers/controllermanager.h"
#include "types/event.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {
namespace {
const float kHonorWeight = 1.5;
const float kNormalWeight = 1;

}  // namespace

REGISTER_PLAYER_CONTROLLER(ThriceBot);

void ThriceBot::RoundStart(std::vector<Piece> _hand, Wind s, Wind p) {
  for (const Piece m : _hand) {
    PlayerTile h;
    h.piece = m;
    if (m.isHonor()) {
      h.weight = kHonorWeight;
    } else {
      h.weight = kNormalWeight;
    }
    hand_.push_back(h);
  }
  assignweights();
  lastEvent_.type = Event::kDiscard;
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

ThriceBot::PlayerTile ThriceBot::assignTileWeight(PlayerTile h1) {
  PlayerTile h;
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

void ThriceBot::ReceiveEvent(Event e) {
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      lastEvent_ = e;
    }

    if (e.type == Event::kDiscard && e.player == pid_) {
      PlayerTile h;
      h.piece = Piece(e.piece);
      if (h.piece.isHonor()) {
        h.weight = kHonorWeight;
      } else {
        h.weight = kNormalWeight;
      }
      h = assignTileWeight(h);
      hand_.push_back(h);
      checkDiscard();
    }

  } else if (e.type == Event::kDiscard) {
    discarded_.at(Piece(e.piece).toUint8_t())++;
  }
}

Event ThriceBot::RetrieveDecision() {
  if (lastEvent_.type == Event::kDiscard) {
    lastEvent_.piece = popDiscard().toUint8_t();
  } else if (lastEvent_.type == Event::kPon) {
    auto p = Piece(lastEvent_.piece);
    if (!checkTile(p)) {
      lastEvent_.type = Event::kDecline;
    }
  } else if (lastEvent_.type == Event::kChi) {
    auto p = Piece(lastEvent_.piece);
    if (!checkTile(p)) {
      lastEvent_.type = Event::kDecline;
    }
  } else if (lastEvent_.type == Event::kRiichi) {
    auto p = Piece(lastEvent_.piece);
    if (discarded_.at(p.toUint8_t()) == 3) {
      lastEvent_.type = Event::kDecline;
    }
  }
  Event e = lastEvent_;
  lastEvent_.type = Event::kDiscard;  // lowest """priority""" event type
  return e;
}

bool ThriceBot::checkTile(Piece p) {
  int j = 0;
  for (const auto& i : hand_) {
    if (i.piece.getSuit() == p.getSuit() &&
        i.piece.getPieceNum() == p.getPieceNum()) {
      j++;
    }
  }
  return j == 2 || j == 3;
}

Piece ThriceBot::popDiscard() {
  if (hand_.empty()) {
    return Piece(kError);
  }
  int index_of_lowest = 0;
  for (size_t i = 0; i < hand_.size(); i++) {
    if (hand_.at(i).weight < hand_[index_of_lowest].weight) {
      index_of_lowest = i;
    }
  }
  const Piece p = hand_[index_of_lowest].piece;
  hand_.erase(hand_.begin() + index_of_lowest);
  discarded_.at(p.toUint8_t())++;
  return p;
}
}  // namespace mahjong
