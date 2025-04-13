#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "playercontroller.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {

using pieceSet = std::map<uint8_t, uint8_t>;

// == Fast Tanyao Bot ==
// - Attempt to get to all simples **quickly**
// - Immediately dump all terminals and honors
// - Call on everything that isn't a terminal or honor
// - Disregard Defense (Where we're going, we don't need defense)
class FastTanyao : public PlayerController {
 public:
  static std::unique_ptr<PlayerController> New() {
    return std::make_unique<FastTanyao>();
  }

  void GameStart(int _playerID) override {}
  void RoundStart(std::vector<Piece> hand, Wind seatWind,
                  Wind prevalentWind) override;
  void ReceiveEvent(Event e) override;
  Event RetrieveDecision() override;

  std::string Name() override { return "FastTanyao"; };

 private:
  void IncrementPiece(Piece piece, pieceSet& set);
  void IncrementPiece(Piece piece, pieceSet& set, uint8_t count);
  static void DecrementPiece(Piece piece, pieceSet& set);
  static bool ShouldKeep(Piece piece);
  void ProcessNewPiece(Piece piece);
  Piece ChooseDiscard();
  void OutputSet(uint8_t id, const pieceSet& set);

  pieceSet possible_triples_;
  std::vector<Piece> immediate_discard_;
  std::vector<Piece> valid_doras_;
  Event decided_decision_;
  pieceSet all_discards_;
};

}  // namespace mahjong
