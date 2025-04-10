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
  std::string Name() override;
  void GameStart(int _playerID) override;
  void RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind,
                  mahjong::Wind prevalentWind) override;
  void ReceiveEvent(mahjong::Event e) override;
  mahjong::Event RetrieveDecision() override;

 private:
  void IncrementPiece(mahjong::Piece piece, pieceSet& set);
  void IncrementPiece(mahjong::Piece piece, pieceSet& set, uint8_t count);
  static void DecrementPiece(mahjong::Piece piece, pieceSet& set);
  static bool ShouldKeep(mahjong::Piece piece);
  void ProcessNewPiece(mahjong::Piece piece);
  mahjong::Piece ChooseDiscard();
  void OutputSet(uint8_t id, const pieceSet& set);
  pieceSet possible_triples_;
  std::vector<mahjong::Piece> immediate_discard_;
  std::vector<mahjong::Piece> valid_doras_;
  mahjong::Event decided_decision_;
  pieceSet all_discards_;
};
