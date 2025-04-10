#pragma once
#include <event.h>
#include <cstddef>
#include <string>
#include <vector>

#include "playercontroller.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

// Always Calls (Angry) and Always Discards the tile it draws

class AngryDiscardoBot : public mahjong::PlayerController {
 public:
  std::string Name() override;
  void GameStart(int _playerID) override;
  void RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind,
                  mahjong::Wind prevalentWind) override;
  void ReceiveEvent(mahjong::Event e) override;
  mahjong::Event RetrieveDecision() override;

 private:
  std::vector<mahjong::Piece> hand_;
  size_t n_ = 0;
  mahjong::Event lastEvent_;
};
