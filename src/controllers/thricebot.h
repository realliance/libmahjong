#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "types/pieces.h"
#include "playercontroller.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

struct HandTile {
  mahjong::Piece piece;
  float weight{};
};

class ThriceBot : public mahjong::PlayerController {
 public:
  std::string Name() override;
  void GameStart(int _playerID) override;
  void RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind,
                  mahjong::Wind prevalentWind) override;
  void ReceiveEvent(mahjong::Event e) override;
  mahjong::Event RetrieveDecision() override;

 private:
  // private functions
  void assignweights();
  mahjong::Piece popDiscard();
  HandTile assignTileWeight(HandTile);
  void checkDiscard();
  bool checkTile(mahjong::Piece);
  // instance vars
  std::vector<HandTile> hand_;
  std::array<uint8_t, mahjong::Piece::kPiecesize> discarded_ = {};
  mahjong::Event lastEvent_;
  mahjong::Wind swind_;
  mahjong::Wind pwind_;
  int pid_;
};
