#pragma once
#include <string>
#include <vector>

#include "controllers/playercontroller.h"
#include "types/piecetype.h"
#include "types/event.h"
#include "types/winds.h"

//Always Calls (Angry) and Always Discards the tile it draws

class TesterBot : public mahjong::PlayerController {
 public:
  std::string Name();
  void GameStart(int _playerID);
  void RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind,
                  mahjong::Wind prevalentWind);
  void ReceiveEvent(mahjong::Event e);
  mahjong::Event RetrieveDecision();
  void AddEvents(std::vector<mahjong::Event> events);
  std::vector<mahjong::Event> GetEvents();

 private:
  std::vector<mahjong::Event> queue;
  std::vector<mahjong::Event> events;
};
