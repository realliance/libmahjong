#pragma once
#include <vector>
#include <array>

#include "event.h"

class MahjongAI{
public:
  virtual auto GameStart(int playerID) -> void = 0;
  virtual auto RoundStart(std::vector<Piece> board, Wind seatWind, Wind prevalentWind) -> void = 0;
  virtual auto ReceiveEvent(Event e) -> void;
  virtual auto RetrieveDecision() -> Event;
};
