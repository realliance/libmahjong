#pragma once
#include <event.h>
#include <piecetype.h>
#include <playercontroller.h>
#include <winds.h>

#include <string>
#include <vector>

// Always Calls (Angry) and Always Discards the tile it draws

class GentlemanBot : public mahjong::PlayerController {
 public:
  auto Name() -> std::string override;
  auto GameStart(int _playerID) -> void override;
  auto RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind,
                  mahjong::Wind prevalentWind) -> void override;
  auto ReceiveEvent(mahjong::Event e) -> void override;
  auto RetrieveDecision() -> mahjong::Event override;

 private:
  std::vector<mahjong::Piece> hand_;
  bool riichi_ = false;

  mahjong::Event lastEvent_;

  auto getDiscard() -> mahjong::Piece;
};
