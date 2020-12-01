#pragma once
#include <string>
#include <vector>
#include <event.h>
#include <playercontroller.h>
#include <piecetype.h>
#include <winds.h>

//Always Calls (Angry) and Always Discards the tile it draws

class TesterBot : public Mahjong::PlayerController{
public:
  auto Name() -> std::string;
  auto GameStart(int _playerID) -> void;
  auto RoundStart(std::vector<Mahjong::Piece> hand, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void;
  auto ReceiveEvent(Mahjong::Event e) -> void;
  auto RetrieveDecision() -> Mahjong::Event;
  auto AddEvents(std::vector<Mahjong::Event> events) -> void;
  auto GetEvents() -> std::vector<Mahjong::Event>;
private:
  std::vector<Mahjong::Event> queue;
  std::vector<Mahjong::Event> events;
};
