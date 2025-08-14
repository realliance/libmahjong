#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "controllers/playercontroller.h"
#include "types/event.h"
#include "types/winds.h"

namespace mahjong {

class PlayerControllerFake : public PlayerController {
 public:
  using DecisionCallback = std::function<Event(const Event&)>;

  static std::unique_ptr<PlayerController> New() {
    return std::make_unique<PlayerControllerFake>();
  }

  // Default constructor has standard decline behavior
  PlayerControllerFake();
  explicit PlayerControllerFake(DecisionCallback callback);

  void GameStart(int _playerID) override {}
  void RoundStart(std::vector<Piece> hand, Wind seatWind,
                  Wind prevalentWind) override {}
  void ReceiveEvent(Event e) override;
  Event RetrieveDecision() override;

  // Set a new decision callback
  void SetDecisionCallback(DecisionCallback callback) {
    decisionCallback_ = callback;
  }

  std::string Name() override { return "PlayerControllerFake"; }

  static Event DefaultDecisionCallback(const Event& e);

 private:
  DecisionCallback decisionCallback_;
  Event lastEvent_;
};

}  // namespace mahjong
