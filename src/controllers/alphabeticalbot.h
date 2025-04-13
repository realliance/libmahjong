#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "playercontroller.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {

// Always Discards Alphabetically.
class AlphabeticalBot : public PlayerController {
 public:
  static std::unique_ptr<PlayerController> New() {
    return std::make_unique<AlphabeticalBot>();
  }

  void GameStart(int player_id) override { id_ = player_id; }
  void RoundStart(std::vector<Piece> hand, Wind seatWind,
                  Wind prevalentWind) override;
  void ReceiveEvent(Event e) override;
  Event RetrieveDecision() override;

  std::string Name() override { return "AlphabeticalBot"; };

 private:
  int getDiscardPiece();

  std::vector<Piece> hand_;
  int id_;
  Event decisionToTake_;
};
}  // namespace mahjong
