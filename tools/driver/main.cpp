#include <atomic>
#include <fstream>
#include <iomanip>
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

const int THREADS = 8;
const int ITERATIONS = 1000000;

auto main() -> int {
  std::atomic<int> branches = 134;
  std::atomic<int> cnt = 0;
  std::atomic<int> sync = 0;
  std::atomic_flag gated = false;
  std::cout << std::fixed << std::setprecision(1);
  auto branchFinder = [&cnt, &branches, &sync, &gated](int thread) {
    std::shared_ptr<Mahjong::Node> root;
    for (int i = 0; i <= ITERATIONS / THREADS; i++) {
      if ((i % (ITERATIONS / 1000)) == 0) {
        sync++;
        if (sync == THREADS) {
          if (!gated.test_and_set()) {
            std::cout << "Progess: " << std::setw(5) << static_cast<double>(i) * 100 * THREADS / ITERATIONS << "%\r" << std::flush;
            sync -= THREADS;
            gated.clear();
          }
        }
      }
      auto hand = Mahjong::GetPossibleStdFormHand();
      root = Mahjong::breakdownHand(hand);
      size_t tempBranches = Mahjong::Node::AsBranchVectors(root.get()).size();
      if (tempBranches > branches) {
        branches = tempBranches;
        std::ofstream os(std::to_string(thread) + "hand.gv");
        root->DumpAsDot(os);
        os.close();
        cnt = 1;
        std::cout << std::endl
                  << "t" << thread << ": "
                  << "branches: " << Mahjong::Node::AsBranchVectors(root.get()).size() << std::endl;
      } else if (tempBranches == branches) {
        std::ofstream os(std::to_string(thread) + "hand.gv");
        root->DumpAsDot(os);
        os.close();
        cnt++;
        std::cout << std::endl
                  << "t" << thread << ": "
                  << "Count: " << cnt << std::endl;
      }
    }
  };
  std::array<std::thread, THREADS> threads;
  for (int i = 0; i < THREADS; i++) {
    threads.at(i) = std::thread(branchFinder, i);
  }
  for (int i = 0; i < THREADS; i++) {
    threads.at(i).join();
  }

  return 0;
}
