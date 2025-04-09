#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "event.h"
#include "pieces.h"
#include "piecetype.h"
#include "playercontroller.h"
#include "winds.h"

struct HandTile {
  mahjong::Piece piece;
  float weight{};
};

class ThriceBot : public mahjong::PlayerController {
 public:
  auto Name() -> std::string override;
  auto GameStart(int _playerID) -> void override;
  auto RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind,
                  mahjong::Wind prevalentWind) -> void override;
  auto ReceiveEvent(mahjong::Event e) -> void override;
  auto RetrieveDecision() -> mahjong::Event override;

 private:
  // private functions
  void assignweights();
  auto popDiscard() -> mahjong::Piece;
  auto assignTileWeight(HandTile) -> HandTile;
  void checkDiscard();
  auto checkTile(mahjong::Piece) -> bool;
  // instance vars
  std::vector<HandTile> hand_;
  std::array<uint8_t, mahjong::Piece::kPiecesize> discarded_ = {};
  mahjong::Event lastEvent_;
  mahjong::Wind swind_;
  mahjong::Wind pwind_;
  int pid_;
};
