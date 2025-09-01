#include "controllers/alphabeticalbot.h"

#include <cstddef>
#include <string>
#include <vector>

#include "controllers/controllermanager.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {
REGISTER_PLAYER_CONTROLLER(AlphabeticalBot);

void AlphabeticalBot::RoundStart(std::vector<Piece> hand, Wind /*seatWind*/,
                                 Wind /*prevalentWind*/) {
  hand_ = hand;
  decisionToTake_.type = Event::kDiscard;
  decisionToTake_.player = id_;
}

void AlphabeticalBot::ReceiveEvent(Event e) {
  // const Piece eventPiece = Piece(e.piece);
  // std::cout << "Player " << id <<" got event " << e << std::endl;
  if (e.type <= Event::kDiscard && e.decision && e.player == id_) {
    if (e.type == Event::kDiscard) {
      if (e.type < decisionToTake_.type) {
        decisionToTake_ = e;
      }
    }
  }
  switch (e.type) {
    case Event::kDora:
      break;
    case Event::kKan:
    case Event::kChi:
    case Event::kPon:
      if (e.decision) {
        decisionToTake_.type = Event::kDecline;
      }
      break;
    case Event::kDiscard:
      if (e.decision && e.player == id_) {
        // std::cout << "Player " << id_ << " pushing piece into hand: " << e.piece << std::endl;
        hand_.emplace_back(e.piece);
      }
      break;
    case Event::kTsumo:
    default:
      if (e.decision) {
        decisionToTake_ = e;
      }
      break;
  }
}

Event AlphabeticalBot::RetrieveDecision() {
  if (hand_.empty()) {
    return decisionToTake_;
  }
  if (decisionToTake_.type == Event::kDiscard) {
    auto index_to_discard = getDiscardPiece();
    decisionToTake_.piece = hand_[index_to_discard].raw_value();
    // std::cout << "Removing piece "<< indexToDiscard <<std::endl;
    hand_.erase(hand_.begin() + index_to_discard);
  }
  auto final = decisionToTake_;
  decisionToTake_.type = Event::kDiscard;
  // std::cout << "Sending decision "<< final <<std::endl;
  return final;
}

// Choose a piece to discard (
int AlphabeticalBot::getDiscardPiece() {
  std::vector<std::string> string_board;
  string_board.reserve(hand_.size());
  for (const auto& i : hand_) {
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

}  // namespace mahjong
