#include <fstream>

#include "analysis.h"
#include "handnode.h"
#include "hands.h"
#include "decisionfunction.h"
#include "walls.h"
#include "gamestate.h"
#include "playercontroller.h"
#include "controllermanager.h"
#include "statefunctions.h"
#include "angrydiscardobot.h"
#include "fasttanyao.h"
#include "gentlemanbot.h"
#include "thricebot.h"

using Mahjong::PlayerController, Mahjong::StartGame, Mahjong::Event;

std::array<int,4> scores = {0,0,0,0};

template<class T> 
class Proxy : public PlayerController{
public:
  auto Name() -> std::string{ return proxied.Name();}
  auto GameStart(int _playerID) -> void{
    // for(int i = 0; i < 4; i++){
    //   scores[i] = 0;
    // }
    return proxied.GameStart(_playerID);
  }
  auto RoundStart(std::vector<Mahjong::Piece> hand, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void{ return proxied.RoundStart(hand, seatWind, prevalentWind);}
  auto ReceiveEvent(Mahjong::Event e) -> void{ 
    if(e.type == Event::PointDiff){
      scores.at(e.player) += e.piece*100;
    }
    return proxied.ReceiveEvent(e);
  }
  auto RetrieveDecision() -> Mahjong::Event{ return proxied.RetrieveDecision();}
private:
  T proxied;
};

auto main() -> int{
  Mahjong::RegisterController([](){return new Proxy<FastTanyao>();},"ProxiedFastTanyao");
  const int ROUNDS = 100;
  for(int i = 0; i < ROUNDS; i++){
    std::cout << i << " " << std::flush;
    StartGame({
      { "GentlemanBot","GentlemanBot","ProxiedFastTanyao","Fast Tanyao"},
      {},
      0
    }, /*async=*/false);
  }
  std::cout << std::endl;
  std::cout << "Scores: ";
  for(int i = 0; i < 4; i++){
    std::cout << scores.at(i)/ROUNDS << ", ";
  }
  std::cout << std::endl;
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
  // state.pendingPiece = Piece::NINE_CHARACTER;
  // state.roundNum = 2;
  // state.hands[0].live = {Piece::NINE_CHARACTER,Piece::NINE_CHARACTER,Piece::NINE_CHARACTER,Piece::ONE_PIN,Piece::TWO_PIN,Piece::THREE_PIN,Piece::EAST_WIND,Piece::EAST_WIND};
  // state.hands[0].melds = {
  //   {
  //     Meld::Chi,
  //     Piece::THREE_BAMBOO
  //   },
  //   {
  //     Meld::Pon,
  //     Piece::SIX_BAMBOO
  //   }
  // };

  // std::cout << (isComplete(state,0) ? "Complete" : "Is Not Complete") << std::endl;


  return 0;
}
