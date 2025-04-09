#include "alphabeticalbot.h"

#include <compare>
#include <cstddef>

#include "event.h"
#include "piecetype.h"
#include "winds.h"

auto AlphabeticalBot::Name() -> std::string {
  return "AlphabeticalBot";
}

auto AlphabeticalBot::GameStart(int id) -> void {
  id_ = id;
  // std::cout << "Started player " << id << std::endl;
}

auto AlphabeticalBot::RoundStart(std::vector<mahjong::Piece> hand,
                                 /*seatWind=*/mahjong::Wind,
                                 /*prevalentWind=*/mahjong::Wind) -> void {
  hand_ = hand;
  decisionToTake_.type = mahjong::Event::kDiscard;
  decisionToTake_.player = id_;
}

auto AlphabeticalBot::ReceiveEvent(mahjong::Event e) -> void {
  // const mahjong::Piece eventPiece = mahjong::Piece(e.piece);
  // std::cout << "Player " << id <<" got event " << e << std::endl;
  if (e.type <= mahjong::Event::kDiscard && e.decision && e.player == id_) {
    if (e.type == mahjong::Event::kDiscard) {
      if (e.type < decisionToTake_.type) {
        decisionToTake_ = e;
      }
    }
  }
  switch (e.type) {
    case mahjong::Event::kDora:
      break;
    case mahjong::Event::kKan:
    case mahjong::Event::kChi:
    case mahjong::Event::kPon:
      if (e.decision) {
        decisionToTake_.type = mahjong::Event::kDecline;
      }
      break;
    case mahjong::Event::kDiscard:
      if (e.decision && e.player == id_) {
        // std::cout << "Player " << id_ << " pushing piece into hand: " << e.piece << std::endl;
        hand_.emplace_back(e.piece);
      }
      break;
    case mahjong::Event::kTsumo:
    default:
      if (e.decision) {
        decisionToTake_ = e;
      }
      break;
  }
}

auto AlphabeticalBot::RetrieveDecision() -> mahjong::Event {
  if (hand_.empty()) {
    return decisionToTake_;
  }
  if (decisionToTake_.type == mahjong::Event::kDiscard) {
    auto index_to_discard = getDiscardPiece();
    decisionToTake_.piece = hand_[index_to_discard].raw_value();
    // std::cout << "Removing piece "<< indexToDiscard <<std::endl;
    hand_.erase(hand_.begin() + index_to_discard);
  }
  auto final = decisionToTake_;
  decisionToTake_.type = mahjong::Event::kDiscard;
  // std::cout << "Sending decision "<< final <<std::endl;
  return final;
}

// Choose a piece to discard (
auto AlphabeticalBot::getDiscardPiece() -> int {
  std::vector<std::string> string_board;
  string_board.reserve(hand_.size());
  for (auto& i : hand_) {
    string_board.push_back(i.toStr());
  }

  int index = 0;
  for (size_t i = 1; i < string_board.size();
       i++)  // find index of first string (alphabetically)
  {
    if (string_board.at(i) < string_board[index]) {
      index = i;
    }
  }
  // std::cout << "getDiscardPiece(): " << index << std::endl;
  return index;
}
