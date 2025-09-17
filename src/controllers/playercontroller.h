#pragma once
#include <string>
#include <vector>

#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {

class PlayerController {
 public:
  virtual ~PlayerController() noexcept = default;
  virtual void GameStart(int player_id) = 0;
  virtual void RoundStart(std::vector<Piece> hand, Wind seat_wind,
                          Wind prevalent_wind) = 0;
  virtual void ReceiveEvent(Event e) = 0;
  virtual Event RetrieveDecision() = 0;
  virtual std::string Name() = 0;
};

}  // namespace mahjong
