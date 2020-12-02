#include "alphabeticalbot.h"

#include <compare>
#include <cstddef>
#include <memory>

#include "event.h"
#include "piecetype.h"
#include "winds.h"

auto AlphabeticalBot::Name() -> std::string {
  return "AlphabeticalBot";
}

auto AlphabeticalBot::GameStart(int id) -> void {
  this->id = id;
  //std::cout << "Started player " << id << std::endl;
}

auto AlphabeticalBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind /*seatWind*/, Mahjong::Wind /*prevalentWind*/) -> void {
  hand = _hand;
  decisionToTake.type = Mahjong::Event::Discard;
  decisionToTake.player = this->id;
}

auto AlphabeticalBot::ReceiveEvent(Mahjong::Event e) -> void {
  // const Mahjong::Piece eventPiece = Mahjong::Piece(e.piece);
  //std::cout << "Player " << id <<" got event " << e << std::endl;
  if (e.type <= Mahjong::Event::Discard && e.decision && e.player == this->id) {
    if (e.type == Mahjong::Event::Discard) {
      if (e.type < decisionToTake.type) {
        decisionToTake = e;
      }
    }
  }
  switch (e.type) {
    case Mahjong::Event::Dora:
      break;
    case Mahjong::Event::Kan:
    case Mahjong::Event::Chi:
    case Mahjong::Event::Pon:
      if (e.decision) {
        decisionToTake.type = Mahjong::Event::Decline;
      }
      break;
    case Mahjong::Event::Discard:
      if (e.decision && e.player == this->id) {
        //std::cout << "Player " << this->id << " pushing piece into hand: " << e.piece << std::endl;
        hand.emplace_back(e.piece);
      }
      break;
    case Mahjong::Event::Tsumo:
    default:
      if (e.decision) {
        decisionToTake = e;
      }
      break;
  }
}

auto AlphabeticalBot::RetrieveDecision() -> Mahjong::Event {
  if (hand.empty()) {
    return this->decisionToTake;
  }
  if (this->decisionToTake.type == Mahjong::Event::Discard) {
    auto indexToDiscard = getDiscardPiece();
    decisionToTake.piece = hand[indexToDiscard].raw_value();
    //std::cout << "Removing piece "<< indexToDiscard <<std::endl;
    hand.erase(hand.begin() + indexToDiscard);
  }
  auto final = this->decisionToTake;
  this->decisionToTake.type = Mahjong::Event::Discard;
  //std::cout << "Sending decision "<< final <<std::endl;
  return final;
}

//Choose a piece to discard (
auto AlphabeticalBot::getDiscardPiece() -> int {
  std::vector<std::string> stringBoard;
  for (auto& i : this->hand) {
    stringBoard.push_back(i.toStr());
  }

  int index = 0;
  for (size_t i = 1; i < stringBoard.size(); i++)  //find index of first string (alphabetically)
  {
    //I have no idea why clang thinks this < should be nullptr we should file an issue
    //NOLINTNEXTLINE(modernize-use-nullptr)
    if (stringBoard.at(i) < stringBoard[index]) {
      index = i;
    }
  }
  //std::cout << "getDiscardPiece(): " << index << std::endl;
  return index;
}
