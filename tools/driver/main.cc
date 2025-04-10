#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "controllers/controllermanager.h"
#include "controllers/fasttanyao.h"
#include "controllers/playercontroller.h"
#include "statefunctions/statecontroller.h"
#include "types/event.h"
#include "types/piecetype.h"
#include "types/settings.h"
#include "types/winds.h"

namespace {

std::array<int, 4> scores = {0, 0, 0, 0};

}

template <class T>
class Proxy : public mahjong::PlayerController {
 public:
  std::string Name() override { return proxied_.Name(); }
  void GameStart(int _playerID) override {
    // for(int i = 0; i < 4; i++){
    //   scores[i] = 0;
    // }
    return proxied_.GameStart(_playerID);
  }
  void RoundStart(std::vector<mahjong::Piece> hand, mahjong::Wind seatWind,
                  mahjong::Wind prevalentWind) override {
    return proxied_.RoundStart(hand, seatWind, prevalentWind);
  }
  void ReceiveEvent(mahjong::Event e) override {
    if (e.type == mahjong::Event::kPointDiff) {
      scores.at(e.player) += e.piece * 100;
    }
    return proxied_.ReceiveEvent(e);
  }
  mahjong::Event RetrieveDecision() override {
    return proxied_.RetrieveDecision();
  }

 private:
  T proxied_;
};

int main() {
  mahjong::RegisterController([]() { return new Proxy<FastTanyao>(); },
                              "ProxiedFastTanyao");
  constexpr int kRounds = 100;
  for (int i = 0; i < kRounds; i++) {
    std::cout << i << " " << std::flush;
    mahjong::StartGame(
        mahjong::GameSettings{
            .seatControllers = {"GentlemanBot", "GentlemanBot",
                                "ProxiedFastTanyao", "Fast Tanyao"},
        },
        /*async=*/false);
  }
  std::cout << '\n';
  std::cout << "Scores: ";
  for (int i = 0; i < 4; i++) {
    std::cout << scores.at(i) / kRounds << ", ";
  }
  std::cout << '\n';
  // GameState state;
  // state.nextState = Discard;
  // state.seed = 0xBEEFBABE;
  // state.g.seed(state.seed);
  // state.walls = Walls(state.g);
  // state.walls.TakeHand();state.walls.TakeHand();state.walls.TakeHand();state.walls.TakeHand();
  // state.walls.TakePiece();state.walls.TakePiece();state.walls.TakePiece();state.walls.TakePiece();state.walls.TakePiece();state.walls.TakePiece();
  // state.currentPlayer = 3;
  // state.turnNum = 40;
  // state.lastCall = 36;
  // state.pendingPiece = kNineCharacter;
  // state.roundNum = 2;
  // state.hands[0].live = {kNineCharacter,kNineCharacter,kNineCharacter,kOnePin,kTwoPin,kThreePin,kEastWind,kEastWind};
  // state.hands[0].melds = {
  //   {
  //     Meld::kChi,
  //     kThreeBamboo
  //   },
  //   {
  //     Meld::kPon,
  //     kSixBamboo
  //   }
  // };

  // std::cout << (isComplete(state,0) ? "Complete" : "Is Not Complete") << std::endl;

  return 0;
}
