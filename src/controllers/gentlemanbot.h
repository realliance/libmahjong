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

// Always attempts to call Riichi.
class GentlemanBot : public PlayerController {
 public:
  static std::unique_ptr<PlayerController> New() {
    return std::make_unique<GentlemanBot>();
  }

  void GameStart(int _playerID) override {}
  void RoundStart(std::vector<Piece> hand, Wind seatWind,
                  Wind prevalentWind) override;
  void ReceiveEvent(Event e) override;
  Event RetrieveDecision() override;

  std::string Name() override { return "TotoBot"; };

 private:
  Piece getDiscard();

  std::vector<Piece> hand_;
  bool riichi_ = false;
  Event lastEvent_;
};

}  // namespace mahjong
