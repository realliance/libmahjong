#include "gentlemanbot.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "analysis.h"
#include "event.h"
#include "handnode.h"
#include "pieces.h"
#include "piecetype.h"
#include "winds.h"

namespace {

void countpieces(std::array<int8_t, mahjong::Piece::kPiecesize> counts,
                 const std::vector<mahjong::Piece>& pieces) {
  for (const auto& p : pieces) {
    counts.at(p.toUint8_t())++;
  }
}

}  // namespace

std::string GentlemanBot::Name() {
  return "GentlemanBot";
}

void GentlemanBot::GameStart(int /*playerID*/) {}

void GentlemanBot::RoundStart(std::vector<mahjong::Piece> hand,
                              mahjong::Wind /*seatWind*/,
                              mahjong::Wind /*prevalentWind*/) {
  hand_ = hand;
  lastEvent_.type = mahjong::Event::kDiscard;
  riichi_ = false;
}

void GentlemanBot::ReceiveEvent(mahjong::Event e) {
  if (e.decision) {
    if (e.type <= lastEvent_.type) {
      lastEvent_ = e;
    }

    if (e.type == mahjong::Event::kDiscard) {
      if (!riichi_) {
        hand_.emplace_back(e.piece);
        lastEvent_.piece = getDiscard().toUint8_t();
      }
    } else {
    }
  }
}

mahjong::Event GentlemanBot::RetrieveDecision() {
  if (lastEvent_.type == mahjong::Event::kRiichi) {
    lastEvent_.type = mahjong::Event::kRiichi;
    riichi_ = true;
  } else if (lastEvent_.type != mahjong::Event::kDiscard) {
    lastEvent_.type = mahjong::Event::kDecline;
  }

  mahjong::Event e = lastEvent_;
  lastEvent_.type =
      mahjong::Event::kDiscard;  // lowest """priority""" event type
  return e;
}

mahjong::Piece GentlemanBot::getDiscard() {
  std::vector<mahjong::Piece> free_pieces;
  std::vector<mahjong::Piece> prefered_discards;
  std::vector<mahjong::Piece> second_tier_discards;
  std::vector<mahjong::Piece> third_tier_discards;

  std::array<int8_t, mahjong::Piece::kPiecesize> counts = {};
  auto symbolic_hand = mahjong::breakdownHand(hand_);
  auto* current_node = symbolic_hand.get();

  while (true) {
    if (current_node->type == mahjong::Node::kSingle) {
      free_pieces.push_back(current_node->start);
    }
    if (current_node->leaves.empty()) {
      break;
    }

    current_node = current_node->leaves[0];
  }

  countpieces(counts, free_pieces);

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
  return mahjong::Piece(mahjong::kError);
}
