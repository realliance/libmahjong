#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "playercontroller.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/winds.h"

namespace mahjong {

class ThriceBot : public PlayerController {
 public:
  struct HandTile {
    Piece piece;
    float weight{};
  };

  static std::unique_ptr<PlayerController> New() {
    return std::make_unique<ThriceBot>();
  }

  void GameStart(int player_id) override { pid_ = player_id; }
  void RoundStart(std::vector<Piece> hand, Wind seatWind,
                  Wind prevalentWind) override;
  void ReceiveEvent(Event e) override;
  Event RetrieveDecision() override;

  std::string Name() override { return "ThriceBot"; };

 private:
  void assignweights();
  Piece popDiscard();
  HandTile assignTileWeight(HandTile h1);
  void checkDiscard();
  bool checkTile(Piece p);

  std::vector<HandTile> hand_;
  std::array<uint8_t, Piece::kPiecesize> discarded_ = {};
  Event lastEvent_;
  Wind swind_;
  Wind pwind_;
  int pid_;
};

}  // namespace mahjong
