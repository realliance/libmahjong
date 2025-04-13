#pragma once
#include <memory>
#include <string>
#include <vector>

#include "playercontroller.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {

class TotoBot : public PlayerController {
 public:
  static std::unique_ptr<PlayerController> New() {
    return std::make_unique<TotoBot>();
  }

  void GameStart(int _playerID) override;
  void RoundStart(std::vector<Piece> hand, Wind seatWind,
                  Wind prevalentWind) override;
  void ReceiveEvent(Event e) override;
  Event RetrieveDecision() override;

  std::string Name() override { return "TotoBot"; };

 private:
  std::vector<Piece> hand_;
  Event lastEvent_;
};

}  // namespace mahjong
