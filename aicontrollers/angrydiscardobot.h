#pragma once
#include <string>
#include <vector>
#include <event.h>
#include <playercontroller.h>
#include <winds.h>
#include <piecetype.h>

//Always Calls (Angry) and Always Discards the tile it draws

class AngryDiscardoBot : public Mahjong::PlayerController{
public:
  auto Name() -> std::string;
  auto GameStart(int _playerID) -> void;
  auto RoundStart(std::vector<Mahjong::Piece> board, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void;
  auto ReceiveEvent(Mahjong::Event e) -> void;
  auto RetrieveDecision() -> Mahjong::Event;
private:
  Mahjong::Event lastEvent;
};
