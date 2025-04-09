#pragma once
#include <string>
#include <vector>
#include <event.h>
#include <playercontroller.h>
#include <piecetype.h>
#include <winds.h>

//Always Calls (Angry) and Always Discards the tile it draws

class TesterBot : public mahjong::PlayerController{
public:
  auto Name() -> std::string;
  auto GameStart(int _playerID) -> void;
  auto RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind, mahjong::Wind prevalentWind) -> void;
  auto ReceiveEvent(mahjong::Event e) -> void;
  auto RetrieveDecision() -> mahjong::Event;
  auto AddEvents(std::vector<mahjong::Event> events) -> void;
  auto GetEvents() -> std::vector<mahjong::Event>;
private:
  std::vector<mahjong::Event> queue;
  std::vector<mahjong::Event> events;
};
