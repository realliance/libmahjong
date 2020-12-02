#pragma once
#include <event.h>
#include <piecetype.h>
#include <playercontroller.h>
#include <winds.h>

#include <string>
#include <vector>

//Always Calls (Angry) and Always Discards the tile it draws

class TotoBot : public Mahjong::PlayerController {
 public:
  auto Name() -> std::string override;
  auto GameStart(int _playerID) -> void override;
  auto RoundStart(std::vector<Mahjong::Piece> hand, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void override;
  auto ReceiveEvent(Mahjong::Event e) -> void override;
  auto RetrieveDecision() -> Mahjong::Event override;

 private:
  std::vector<Mahjong::Piece> hand;
  Mahjong::Event lastEvent;
};
