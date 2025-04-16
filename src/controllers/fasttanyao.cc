#include "fasttanyao.h"

#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

#include "controllermanager.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {
REGISTER_PLAYER_CONTROLLER(FastTanyao);

bool FastTanyao::ShouldKeep(Piece piece) {
  return !piece.isHonor() && !piece.isTerminal();
}

void FastTanyao::OutputSet(uint8_t /*unused*/, const pieceSet& /*unused*/) {
  // std::cout << "(" << Piece(id).toStr() << ", " << unsigned(set.at(id)) << ")" << std::endl;
}

void FastTanyao::IncrementPiece(Piece piece, pieceSet& set) {
  IncrementPiece(piece, set, /*count=*/1);
}

void FastTanyao::IncrementPiece(Piece piece, pieceSet& set,
                                uint8_t /*unused*/) {
  auto set_contains_piece = set.find(piece.raw_value());
  if (set_contains_piece != set.end()) {
    set_contains_piece->second++;
    OutputSet(piece.raw_value(), set);
    return;
  }

  set.insert(std::pair<uint8_t, uint8_t>(piece.raw_value(), 1));
  OutputSet(piece.raw_value(), set);
}

void FastTanyao::DecrementPiece(Piece piece, pieceSet& set) {
  auto set_contains_piece = set.find(piece.raw_value());
  if (set_contains_piece != set.end()) {
    set_contains_piece->second--;
    if (set_contains_piece->second == 0) {
      set.erase(set_contains_piece);
    }
    return;
  }
}

void FastTanyao::ProcessNewPiece(Piece piece) {
  if (!ShouldKeep(piece)) {
    immediate_discard_.push_back(piece);
    // std::cout << Piece(piece).toStr() << " should be immediately discarded." << std::endl;
    return;
  }

  IncrementPiece(piece, possible_triples_);
}

void FastTanyao::RoundStart(std::vector<Piece> _hand, Wind /*seatWind*/,
                            Wind /*prevalentWind*/) {
  for (auto piece : _hand) {
    ProcessNewPiece(piece);
  }
  decided_decision_.type = Event::kDiscard;
}

Piece FastTanyao::ChooseDiscard() {
  if (!immediate_discard_.empty()) {
    auto discard = immediate_discard_.back();
    immediate_discard_.pop_back();
    // std::cout << "Chose piece " << discard.toStr() << " to discard (imm discard)" << std::endl;
    return discard;
  }

  pieceSet discard_options;

  // Document pieces in play, if anything adds up to 4 (nothing left), discard.
  for (const auto& possible_triple : possible_triples_) {
    auto piece = possible_triple.first;
    auto set_contains_piece = all_discards_.find(piece);
    if (set_contains_piece != all_discards_.end()) {
      if (possible_triple.second + set_contains_piece->second == 4 &&
          possible_triple.second < 3) {
        DecrementPiece(Piece(piece), possible_triples_);
        // std::cout << "Chose piece " << Piece(piece).toStr() << " to discard (all pieces in play)" << std::endl;
        return Piece(piece);
      }
      if (possible_triple.second > 0) {
        IncrementPiece(Piece(piece), discard_options, possible_triple.second);
      }
    }
  }

  auto min_value = *std::ranges::min_element(
      discard_options,
      [](const auto& l, const auto& r) { return l.second < r.second; });
  // std::cout << unsigned(minValue.first) << ", " << unsigned(minValue.second) << std::endl;
  auto discard_piece = Piece(min_value.first);
  DecrementPiece(discard_piece, possible_triples_);
  // std::cout << "Chose piece " << discardPiece.toStr() << " to discard (min risk)" << std::endl;
  return discard_piece;
}

void FastTanyao::ReceiveEvent(Event e) {
  const Piece event_piece = Piece(e.piece);
  if (e.type <= Event::kDiscard && e.decision) {
    if (e.type == Event::kDiscard || ShouldKeep(Piece(e.piece))) {
      if (e.type < decided_decision_.type) {
        // std::cout << "Choosing decision " << e.type << std::endl;
        decided_decision_ = e;
      }
    }
  }

  switch (e.type) {
    case Event::kDora:
      if (ShouldKeep(event_piece)) {
        valid_doras_.push_back(event_piece);
      }
      break;
    case Event::kPon:
      IncrementPiece(event_piece, all_discards_, /*count=*/3);
      break;
    case Event::kKan:
      IncrementPiece(event_piece, all_discards_, /*count=*/4);
      break;
    case Event::kChi:
      IncrementPiece(event_piece, all_discards_);
      IncrementPiece(event_piece + 1, all_discards_);
      IncrementPiece(event_piece + 2, all_discards_);
      break;
    case Event::kDiscard:
      if (e.decision) {
        ProcessNewPiece(event_piece);
      } else {
        IncrementPiece(event_piece, all_discards_);
      }
      break;
    default:
      break;
  }
}

Event FastTanyao::RetrieveDecision() {
  if (decided_decision_.type == Event::kDiscard) {
    decided_decision_.piece = ChooseDiscard().raw_value();
  }

  auto final_event = decided_decision_;
  decided_decision_.type = Event::kDiscard;
  // std::cout << "Sending Decision: " << finalEvent << std::endl;
  return final_event;
}
}  // namespace mahjong
