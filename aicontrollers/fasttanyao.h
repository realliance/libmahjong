#pragma once
#include <event.h>
#include <piecetype.h>
#include <playercontroller.h>
#include <winds.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

/**
== Fast Tanyao Bot ==
- Attempt to get to all simples **quickly**
- Immediately dump all terminals and honors
- Call on everything that isn't a terminal or honor
- Disregard Defense (Where we're going, we don't need defense)
**/

using pieceSet = std::map<uint8_t, uint8_t>;

class FastTanyao : public Mahjong::PlayerController {
 public:
  auto Name() -> std::string override;
  auto GameStart(int _playerID) -> void override;
  auto RoundStart(std::vector<Mahjong::Piece> hand, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void override;
  auto ReceiveEvent(Mahjong::Event e) -> void override;
  auto RetrieveDecision() -> Mahjong::Event override;

 private:
  auto IncrementPiece(Mahjong::Piece piece, pieceSet& set) -> void;
  auto IncrementPiece(Mahjong::Piece piece, pieceSet& set, uint8_t count) -> void;
  static auto DecrementPiece(Mahjong::Piece piece, pieceSet& set) -> void;
  static auto ShouldKeep(Mahjong::Piece piece) -> bool;
  auto ProcessNewPiece(Mahjong::Piece piece) -> void;
  auto ChooseDiscard() -> Mahjong::Piece;
  auto OutputSet(uint8_t id, const pieceSet& set) -> void;
  pieceSet possibleTriples;
  std::vector<Mahjong::Piece> immediateDiscard;
  std::vector<Mahjong::Piece> validDoras;
  Mahjong::Event decidedDecision;
  pieceSet allDiscards;
};
