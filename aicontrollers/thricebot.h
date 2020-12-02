#pragma once
#include <event.h>
#include <piecetype.h>
#include <playercontroller.h>
#include <winds.h>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

struct handTile {
  Mahjong::Piece piece;
  float weight{};
};

class ThriceBot : public Mahjong::PlayerController {
 public:
  auto Name() -> std::string override;
  auto GameStart(int _playerID) -> void override;
  auto RoundStart(std::vector<Mahjong::Piece> hand, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void override;
  auto ReceiveEvent(Mahjong::Event e) -> void override;
  auto RetrieveDecision() -> Mahjong::Event override;

 private:
  //private functions
  void assignweights();
  auto popDiscard() -> Mahjong::Piece;
  auto assignTileWeight(handTile) -> handTile;
  void checkDiscard();
  auto checkTile(Mahjong::Piece) -> bool;
  //instance vars
  std::vector<handTile> hand;
  std::array<uint8_t, Mahjong::Piece::PIECESIZE> discarded = {};
  Mahjong::Event lastEvent;
  Mahjong::Wind swind;
  Mahjong::Wind pwind;
  int pid;
};
