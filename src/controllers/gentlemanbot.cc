#include "gentlemanbot.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "analysis/analysis.h"
#include "controllermanager.h"
#include "types/event.h"
#include "types/handnode.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {
REGISTER_PLAYER_CONTROLLER(GentlemanBot);

namespace {
void CountPieces(std::array<int8_t, Piece::kPiecesize> counts,
                 const std::vector<Piece>& pieces) {
  for (const auto& p : pieces) {
    counts.at(p.toUint8_t())++;
  }
}

}  // namespace

void GentlemanBot::RoundStart(std::vector<Piece> hand, Wind /*seatWind*/,
                              Wind /*prevalentWind*/) {
  hand_ = hand;
  lastEvent_.type = Event::kDiscard;
  riichi_ = false;
}

void GentlemanBot::ReceiveEvent(Event e) {
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      lastEvent_ = e;
    }

    if (e.type == Event::kDiscard) {
      if (!riichi_) {
        hand_.emplace_back(e.piece);
        lastEvent_.piece = getDiscard().toUint8_t();
      }
    } else {
    }
  }
}

Event GentlemanBot::RetrieveDecision() {
  if (lastEvent_.type == Event::kRiichi) {
    lastEvent_.type = Event::kRiichi;
    riichi_ = true;
  } else if (lastEvent_.type != Event::kDiscard) {
    lastEvent_.type = Event::kDecline;
  }

  Event e = lastEvent_;
  lastEvent_.type = Event::kDiscard;  // lowest """priority""" event type
  return e;
}

Piece GentlemanBot::getDiscard() {
  std::vector<Piece> free_pieces;
  std::vector<Piece> prefered_discards;
  std::vector<Piece> second_tier_discards;
  std::vector<Piece> third_tier_discards;

  std::array<int8_t, Piece::kPiecesize> counts = {};
  const std::unique_ptr<Node> symbolic_hand = breakdownHand(hand_);

  for (const auto& leaf : *symbolic_hand) {
    if (leaf.type() == Node::kSingle) {
      free_pieces.push_back(leaf.start());
    }
  }

  CountPieces(counts, free_pieces);

  for (const auto& p : free_pieces) {
    if (counts.at((p + 1).toUint8_t()) > 0 ||
        counts.at((p - 1).toUint8_t()) > 0) {
      second_tier_discards.push_back(p);
    } else if (counts.at(p.toUint8_t()) == 2 ||
               counts.at((p + 2).toUint8_t()) > 0 ||
               counts.at((p - 2).toUint8_t()) > 0) {
      third_tier_discards.push_back(p);
    } else {
      prefered_discards.push_back(p);
    }
  }

  for (const auto& p : prefered_discards) {
    if (p.isHonor()) {
      for (size_t i = 0; i < hand_.size(); i++) {
        if (hand_.at(i).toUint8_t() == p.toUint8_t()) {
          hand_.erase(hand_.begin() + i);
        }
      }
      return p;
    }
  }
  for (const auto& p : prefered_discards) {
    for (size_t i = 0; i < hand_.size(); i++) {
      if (hand_.at(i).toUint8_t() == p.toUint8_t()) {
        hand_.erase(hand_.begin() + i);
      }
    }
    return p;
  }
  for (const auto& p : second_tier_discards) {
    for (size_t i = 0; i < hand_.size(); i++) {
      if (hand_.at(i).toUint8_t() == p.toUint8_t()) {
        hand_.erase(hand_.begin() + i);
      }
    }
    return p;
  }
  for (const auto& p : third_tier_discards) {
    for (size_t i = 0; i < hand_.size(); i++) {
      if (hand_.at(i).toUint8_t() == p.toUint8_t()) {
        hand_.erase(hand_.begin() + i);
      }
    }
    return p;
  }
  return Piece(kError);
}

}  // namespace mahjong
