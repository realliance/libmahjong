#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "playercontroller.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {

// Always Calls (Angry) and Always Discards the tile it draws
class AngryDiscardoBot : public PlayerController {
 public:
  static std::unique_ptr<PlayerController> New() {
    return std::make_unique<AngryDiscardoBot>();
  }

  void GameStart(int _playerID) override {}
  void RoundStart(std::vector<Piece> hand, Wind  /*seatWind*/,
                  Wind  /*prevalentWind*/) override {
    hand_ = hand;
    lastEvent_.type = Event::kDiscard;
  }

  void ReceiveEvent(Event e) override;
  Event RetrieveDecision() override;

  std::string Name() override { return "AngryDiscardoBot"; };

 private:
  std::vector<Piece> hand_;
  size_t n_ = 0;
  Event lastEvent_;
};

}  // namespace mahjong
