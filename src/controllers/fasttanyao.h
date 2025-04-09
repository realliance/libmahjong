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

class FastTanyao : public mahjong::PlayerController {
 public:
  auto Name() -> std::string override;
  auto GameStart(int _playerID) -> void override;
  auto RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind,
                  mahjong::Wind prevalentWind) -> void override;
  auto ReceiveEvent(mahjong::Event e) -> void override;
  auto RetrieveDecision() -> mahjong::Event override;

 private:
  auto IncrementPiece(mahjong::Piece piece, pieceSet& set) -> void;
  auto IncrementPiece(mahjong::Piece piece, pieceSet& set, uint8_t count)
      -> void;
  static auto DecrementPiece(mahjong::Piece piece, pieceSet& set) -> void;
  static auto ShouldKeep(mahjong::Piece piece) -> bool;
  auto ProcessNewPiece(mahjong::Piece piece) -> void;
  auto ChooseDiscard() -> mahjong::Piece;
  auto OutputSet(uint8_t id, const pieceSet& set) -> void;
  pieceSet possible_triples_;
  std::vector<mahjong::Piece> immediate_discard_;
  std::vector<mahjong::Piece> valid_doras_;
  mahjong::Event decided_decision_;
  pieceSet all_discards_;
};
