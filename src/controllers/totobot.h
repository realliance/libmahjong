#pragma once
#include <event.h>
#include <piecetype.h>
#include <playercontroller.h>
#include <winds.h>

#include <string>
#include <vector>

// Always Calls (Angry) and Always Discards the tile it draws

class TotoBot : public mahjong::PlayerController {
 public:
  std::string Name() override;
  void GameStart(int _playerID) override;
  void RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind,
                  mahjong::Wind prevalentWind) override;
  void ReceiveEvent(mahjong::Event e) override;
  mahjong::Event RetrieveDecision() override;

 private:
  std::vector<mahjong::Piece> hand_;
  mahjong::Event lastEvent_;
};
