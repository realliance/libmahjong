#pragma once
#include <event.h>
#include <string>
#include <vector>

#include "playercontroller.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

// Always Calls (Angry) and Always Discards the tile it draws

class AlphabeticalBot : public mahjong::PlayerController {
 public:
  std::string Name() override;
  void GameStart(int _playerID) override;
  void RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind,
                  mahjong::Wind prevalentWind) override;
  void ReceiveEvent(mahjong::Event e) override;
  mahjong::Event RetrieveDecision() override;

 private:
  std::vector<mahjong::Piece> hand_;
  int id_;
  int getDiscardPiece();
  mahjong::Event decisionToTake_;
};
