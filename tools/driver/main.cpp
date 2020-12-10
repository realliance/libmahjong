#include <atomic>
#include <fstream>
#include <iomanip>
#include <set>
#include <thread>

#include "analysis.h"
#include "angrydiscardobot.h"
#include "controllermanager.h"
#include "decisionfunction.h"
#include "fasttanyao.h"
#include "gamestate.h"
#include "gentlemanbot.h"
#include "handnode.h"
#include "hands.h"
#include "playercontroller.h"
#include "statefunctions.h"
#include "thricebot.h"
#include "walls.h"

using Mahjong::Piece, Mahjong::breakdownHand, Mahjong::Node, Mahjong::GetPossibleTenpaiHand, Mahjong::isInTenpai, Mahjong::GetPossibleStdFormHand;

void countPieces(const std::vector<Piece>& hand, std::array<uint8_t, Piece::PIECESIZE>* cnts) {
  for (const auto& p : hand) {
    cnts->at(p.toUint8_t())++;
  }
}

auto isInTenpaiFast(const std::vector<Piece>& hand) -> std::vector<Piece> {
  auto root = breakdownHand(hand);
  std::set<Piece> waits;
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    std::vector<Piece> singles;
    int singleNum = 0;
    Piece pair = Piece::ERROR;
    std::array<uint8_t, Piece::PIECESIZE> singlesCnts = {};
    for (auto piece = branch.rbegin(); piece != branch.rend(); ++piece) {
      if ((*piece)->type == Node::Single) {
        if (singlesCnts.at(((*piece)->start.toUint8_t())) == 0) {
          singles.push_back((*piece)->start);
        }
        singleNum++;
        singlesCnts.at(((*piece)->start.toUint8_t()))++;
      } else if ((*piece)->type == Node::Pair) {
        pair = (*piece)->start;
      } else if (pair != Piece::ERROR) {
        break;
      }
    }
    if (singleNum > 3) {
      continue;
    }
    std::array<uint8_t, Piece::PIECESIZE> cnts = {};
    countPieces(hand, &cnts);
    if (singleNum == 2) {
      if (cnts.at(singles[0].toUint8_t()) < 4) {
        std::cout << 1;
        waits.emplace(Piece(singles[0]));
      }
      if (cnts.at(singles[1].toUint8_t()) < 4) {
        std::cout << 2;
        waits.emplace(Piece(singles[1]));
      }
      if (pair != Piece::ERROR && cnts.at(pair.toUint8_t()) < 4) {
        std::cout << 3;
        waits.emplace(Piece(pair));
      }
      continue;
    }
    if (singles.size() == 2) {
      if (singlesCnts.at(singles[0].toUint8_t()) == 2 && cnts.at(singles[0].toUint8_t()) < 4) {
        std::cout << 4;
        waits.emplace(Piece(singles[0]));
      }
      if (singlesCnts.at(singles[1].toUint8_t()) == 2 && cnts.at(singles[1].toUint8_t()) < 4) {
        std::cout << 5;
        waits.emplace(Piece(singles[1]));
      }
      if (pair != Piece::ERROR && cnts.at(pair.toUint8_t()) < 4) {
        std::cout << 6;
        waits.emplace(Piece(pair));
      }
      continue;
    }
    std::sort(singles.begin(), singles.end());
    for (const auto& single : singles) {
      if (single.isHonor()) {
        continue;
      }
      if (singlesCnts.at((single + 1).toUint8_t()) > 0) {
        if ((single + 2) != Piece::ERROR && cnts.at((single + 2).toUint8_t()) < 4) {
          std::cout << 7;
          waits.emplace(single + 2);
        }
        if ((single - 1) != Piece::ERROR && cnts.at((single - 1).toUint8_t()) < 4) {
          std::cout << 8;
          waits.emplace(single - 1);
        }
      }
      if (singlesCnts.at((single + 2).toUint8_t()) > 0) {
        if ((single + 1) != Piece::ERROR && cnts.at((single + 1).toUint8_t()) < 4) {
          std::cout << 9;
          waits.emplace(single + 1);
        }
      }
    }
    if (pair.isHonor()) {
      continue;
    }
    if (singlesCnts.at((pair - 1).toUint8_t()) > 0) {
      if ((pair - 2) != Piece::ERROR && cnts.at((pair - 2).toUint8_t()) < 4) {
        std::cout << 10;
        waits.emplace(pair - 2);
      }
      if ((pair + 1) != Piece::ERROR && cnts.at((pair + 1).toUint8_t()) < 4) {
        std::cout << 11;
        waits.emplace(pair + 1);
      }
    }
    if (singlesCnts.at((pair - 2).toUint8_t()) > 0) {
      if ((pair - 1) != Piece::ERROR && cnts.at((pair - 1).toUint8_t()) < 4) {
        std::cout << 12;
        waits.emplace(pair - 1);
      }
    }
    // if (singlesCnts.at((pair + 1).toUint8_t()) > 0) {
    //   if ((pair + 2) != Piece::ERROR && cnts.at((pair + 2).toUint8_t()) < 4) {
    //     std::cout << 12;
    //     waits.emplace(pair + 2);
    //   }
    //   if ((pair - 1) != Piece::ERROR && cnts.at((pair - 1).toUint8_t()) < 4) {
    //     std::cout << 13;
    //     waits.emplace(pair - 1);
    //   }
    // }
    // if (singlesCnts.at((pair + 2).toUint8_t()) > 0) {
    //   if ((pair + 1) != Piece::ERROR && cnts.at((pair + 1).toUint8_t()) < 4) {
    //     std::cout << 14;
    //     waits.emplace(pair + 1);
    //   }
    // }
  }
  std::cout << std::endl;
  return std::vector(waits.begin(), waits.end());
}

auto main() -> int {
  const int ITERATIONS = 100000;
  const int PROGRESS = ITERATIONS / 1000;
  for (int i = 0; i < ITERATIONS; i++) {
    if ((i + 1) % PROGRESS == 0) {
      std::cout << i + 1 << std::endl;
    }
    /*
    auto hand = GetPossibleStdFormHand();
    auto root = breakdownHand(hand);
    if (!root->IsComplete()) {
      std::sort(hand.begin(), hand.end());
      std::cout << "Hand: ";
      for (const auto& piece : hand) {
        std::cout << piece.toStr() << ", ";
      }
      std::cout << std::endl;
      auto root = breakdownHand(hand);
      std::ofstream os("hand.gv");
      root->DumpAsDot(os);
      os.close();
      std::cout << std::endl;
      throw -1;
    }
    //**/
    //*
    Piece removed;
    auto hand = GetPossibleTenpaiHand(true, &removed);
    auto waits = isInTenpaiFast(hand);
    //*/
    /*
    if (waits.empty()) {
      std::cout << "Removed: " << removed.toStr() << std::endl;
      std::cout << "Hand: ";
      for (const auto& piece : hand) {
        std::cout << piece.toStr() << ", ";
      }
      std::cout << std::endl;
      auto root = breakdownHand(hand);
      std::ofstream os("hand.gv");
      root->DumpAsDot(os);
      os.close();
      std::cout << std::endl;
      return -1;
    }
    //*/
    //*
    bool match = true;
    auto slowwaits = isInTenpai(hand, true);
    if (waits.size() != slowwaits.size()) {
      match = false;
    } else {
      std::sort(waits.begin(), waits.end());
      std::sort(slowwaits.begin(), slowwaits.end());
      for (size_t i = 0; i < waits.size(); i++) {
        if (waits[i] != slowwaits[i]) {
          match = false;
        }
      }
    }
    if (!match) {
      std::cout << "Removed: " << removed.toStr() << std::endl;
      std::cout << "Hand: ";
      for (const auto& piece : hand) {
        std::cout << piece.toStr() << ", ";
      }
      std::cout << std::endl;
      std::cout << "Waits:     ";
      for (const auto& wait : waits) {
        std::cout << wait.toStr() << ", ";
      }
      std::cout << std::endl;
      std::cout << "SlowWaits: ";
      for (const auto& wait : slowwaits) {
        std::cout << wait.toStr() << ", ";
      }
      std::cout << std::endl;
      auto root = breakdownHand(hand);
      std::ofstream os("hand.gv");
      root->DumpAsDot(os);
      os.close();
      std::cout << std::endl;
      return -1;
    }
    //*/
  }
  // Piece removed;
  // auto hand = GetPossibleStdFormHand();
  // auto root = breakdownHand(hand, 4);
  // std::cout << "Hand: ";
  // for (const auto& piece : hand) {
  //   std::cout << piece.toStr() << ", ";
  // }
  // std::ofstream os("hand.gv");
  // root->DumpAsDot(os);
  // os.close();

  return 0;
}
