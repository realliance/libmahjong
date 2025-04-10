#pragma once
#include <array>
#include <string>
#include <vector>

#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {

class PlayerController {
 public:
  virtual ~PlayerController() = default;
  virtual void GameStart(int playerID) = 0;
  virtual void RoundStart(std::vector<Piece> hand, Wind seatWind,
                          Wind prevalentWind) = 0;
  virtual void ReceiveEvent(Event e) = 0;
  virtual Event RetrieveDecision() = 0;
  virtual std::string Name() = 0;
};

}  // namespace mahjong
