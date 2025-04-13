#pragma once
#include <memory>
#include <string>
#include <vector>

#include "controllers/playercontroller.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {

// Fake PlayController for testing.
class PlayerControllerFake : public PlayerController {
 public:
  static std::unique_ptr<PlayerController> New() {
    return std::make_unique<PlayerControllerFake>();
  }

  void GameStart(int _playerID) override {}
  void RoundStart(std::vector<Piece> hand, Wind seatWind,
                  Wind prevalentWind) override {}
  void ReceiveEvent(Event e) override;
  Event RetrieveDecision() override;

  void AddEvents(std::vector<Event> events);
  std::vector<Event> GetEvents();

  std::string Name() override { return "PlayerControllerFake"; };

 private:
  std::vector<Event> queue;
  std::vector<Event> events;
};

}  // namespace mahjong
