#pragma once
#include <event.h>
#include "playercontroller.h"
#include "types/piecetype.h"
#include "types/winds.h"

#include <cstddef>
#include <string>
#include <vector>

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
