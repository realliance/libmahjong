#include "analysis/scoring.h"
#include <algorithm>
#include <vector>

#include "analysis/analysis.h"
#include "analysis/util.h"
#include "analysis/yakus.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/score.h"

namespace mahjong {

namespace {
const int kLowHanBasicPoints = 2000;
const int kManganBasicPoints = 2000;
const int kManganMinHan = 5;
const int kHanemanBasicPoints = 3000;
const int kHanemanMinHan = 6;
const int kBaimanBasicPoints = 4000;
const int kBaimanMinHan = 8;
const int kSanbaimanBasicPoints = 6000;
const int kSanbaimanMinHan = 11;
const int kYakumanBasicPoints = 8000;

const int kPinfuDiscard = 30;
const int kPinfuSelfdraw = 20;
const int kConcealedDiscard = 25;
const int kOpenOrSelfdraw = 20;

const int kSimplepon = 2;
const int kTermHonorpon = 4;
const int kSimplekan = 8;
const int kTermHonorkan = 16;

const int kConcealedMul = 2;
const int kCsimplepon = kSimplepon * kConcealedMul;
const int kCtermHonorpon = kTermHonorpon * kConcealedMul;
const int kCsimplekan = kSimplekan * kConcealedMul;
const int kCtermHonorkan = kTermHonorkan * kConcealedMul;

const int kDragonseatprevalentwind = 2;
const int kEdgeclosedpairwait = 2;
const int kSelfdraw = 2;
const int kOpenpinfu = 2;

const int kLowedgewait = 3;
const int kHighedgewait = 7;

const int kFuRounding = 10;

const std::vector<mahjong::yakuFunc> kYakuFunctions = {
    isFullyConcealedHand,
    isPinfu,
    isPureDoubleChi,
    isAllSimples,
    isMixedTripleChi,
    isPureStraight,
    isPrevalentWind,
    isSeatWind,
    isGreenDragon,
    isWhiteDragon,
    isRedDragon,
    isOutsideHand,
    isAfterAKan,
    isRobbingAKan,
    isBottomOfTheSea,
    isSevenPairs,
    isTriplePon,
    isThreeConcealedPons,
    isThreeKans,
    isAllPons,
    isHalfFlush,
    isLittleThreeDragons,
    isAllTerminalsAndHonors,
    isTerminalsInAllSets,
    isTwicePureDoubleChi,
    isBlessingOfMan,
    isFullFlush,
};
const std::vector<yakuFunc> kYakumanFunctions = {
    isThirteenOrphans, isNineGates,        isBlessingOfHeaven,
    isBlessingOfEarth, isFourConcealedPon, isFourKans,
    isAllGreen,        isAllTerminals,     isAllHonors,
    isBigThreeDragons, isLittleFourWinds,  isBigFourWinds,
    isMaxBranches,
};

}  // namespace

Score scoreHand(const GameState& state, int player) {
  auto root = breakdownHand(state.hands.at(player).live);
  Score s;
  s.han = 0;
  s.yakuman = 0;
  s.fu = 0;
  if (!root->IsComplete() && !isThirteenOrphans(state, player)) {
    return s;
  }
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    Score branchscore;
    if (std::ranges::any_of(branch, [](const auto& node) {
          return node->type() == Node::kSingle;
        })) {
      continue;
    }

    for (const auto& yaku_function : kYakuFunctions) {
      branchscore.han += yaku_function(state, player, branch);
    }
    for (const auto& yaku_function : kYakumanFunctions) {
      branchscore.yakuman += yaku_function(state, player, branch);
    }
    for (const auto& dora : state.walls.GetDoras()) {
      for (const auto& p : state.hands.at(player).live) {
        if (p == dora) {
          branchscore.han++;
        }
      }
      for (const auto& meld : state.hands.at(player).melds) {
        if (meld.start == dora) {
          branchscore.han++;
        }
        if (meld.type == Meld::kChi) {
          if (meld.start + 1 == dora) {
            branchscore.han++;
          }
          if (meld.start + 2 == dora) {
            branchscore.han++;
          }
        }
      }
    }
    branchscore.fu = getFu(state, player, branch);
    if (getBasicPoints(branchscore) > getBasicPoints(s)) {
      s = branchscore;
    }
  }

  return s;
}

int getBasicPoints(Score s) {
  if (s.yakuman > 0) {
    return s.yakuman * kYakumanBasicPoints;
  }
  if (s.han >= kManganMinHan) {
    if (s.han >= kSanbaimanMinHan) {
      return kSanbaimanBasicPoints;
    }
    if (s.han >= kBaimanMinHan) {
      return kBaimanBasicPoints;
    }
    if (s.han >= kHanemanMinHan) {
      return kHanemanBasicPoints;
    }
    if (s.han >= kSanbaimanMinHan) {
      return kSanbaimanBasicPoints;
    }
    if (s.han >= kManganMinHan) {
      return kManganBasicPoints;
    }
  }
  const int p = s.fu * (2 << (1 + s.han));
  if (p > kLowHanBasicPoints) {
    return kLowHanBasicPoints;
  }
  return p;
}

const int kSevenpairs = 25;

int getFu(const GameState& state, int player,
          const std::vector<const mahjong::Node*>& branch) {
  if (isSevenPairs(state, player, branch)) {
    return kSevenpairs;
  }
  if (isPinfu(state, player, branch)) {
    if (state.hasRonned.at(player)) {
      return kPinfuDiscard;
    }
    return kPinfuSelfdraw;
  }
  int fu = 0;
  if (!state.hands.at(player).open && state.hasRonned.at(player)) {
    fu = kConcealedDiscard;
  } else {
    fu = kOpenOrSelfdraw;
  }
  const bool open = state.hands.at(player).open;
  if (isOpenPinfu(state, player, branch)) {
    fu += kOpenpinfu;
  } else if (!state.hasRonned.at(player)) {
    fu += kSelfdraw;
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type == Meld::kKan) {
      if (!meld.start.isHonor() && !meld.start.isTerminal()) {
        fu += open ? kSimplekan : kCsimplekan;
      } else {
        fu += open ? kTermHonorkan : kCtermHonorkan;
      }
    }
  }
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet) {
      if (!node->start().isHonor() && !node->start().isTerminal()) {
        fu += open ? kSimplepon : kCsimplepon;
      } else {
        fu += open ? kTermHonorpon : kCtermHonorpon;
      }
    }
    if (open) {
      continue;
    }
    if (node->type() == Node::kPair) {
      if (node->start().isHonor()) {
        if (node->start() == kGreenDragon || node->start() == kRedDragon ||
            node->start() == kWhiteDragon ||
            node->start() == (state.roundNum > 3 ? kSouthWind : kEastWind)) {
          fu += kDragonseatprevalentwind;
        }
      }
      if (node->start() == state.pendingPiece) {
        fu += kEdgeclosedpairwait;
      }
    }
    if (node->type() == Node::kChiSet) {
      if (node->start() + 1 == state.pendingPiece) {
        fu += kEdgeclosedpairwait;
      }
      if (node->start().getPieceNum() == 1 &&
          state.pendingPiece.getPieceNum() == kLowedgewait &&
          node->start().getSuit() == state.pendingPiece.getSuit()) {
        fu += kEdgeclosedpairwait;
      }
      if (node->start().getPieceNum() == kHighedgewait &&
          state.pendingPiece == node->start()) {
        fu += kEdgeclosedpairwait;
      }
    }
  }
  if ((fu % kFuRounding) != 0) {
    return fu;
  }
  return fu +
         (kFuRounding - (fu % kFuRounding));  // rounding up to multiple of ten
}

bool isOpenPinfu(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch) {
  for (const auto& node : branch) {
    if (node->type() == Node::kPonSet) {
      return false;
    }
    if (node->type() == Node::kPair) {
      if (node->start() == kRedDragon || node->start() == kWhiteDragon ||
          node->start() == kGreenDragon) {
        return false;
      }
      if (node->start() == kSouthWind && state.roundNum > 3) {
        return false;
      }
      if (node->start() == kEastWind && state.roundNum < 4) {
        return false;
      }
    }
  }
  for (const auto& meld : state.hands.at(player).melds) {
    if (meld.type > Meld::kChi) {
      return false;
    }
  }
  auto waits = isInTenpai(state.hands.at(player).live, /*allWaits=*/true);
  return waits.size() != 1;
}

bool isComplete(const GameState& state, int player) {
  auto root = breakdownHand(state.hands.at(player).live);
  if (!root->IsComplete() && !isThirteenOrphans(state, player)) {
    return false;
  }
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (std::any_of(branch.begin(), branch.end(), [](const auto& node) {
          return node->type() == Node::kSingle;
        })) {
      continue;
    }

    for (const auto& yaku_function : kYakuFunctions) {
      if (yaku_function(state, player, branch) > 0) {
        return true;
      }
    }
    for (const auto& yaku_function : kYakumanFunctions) {
      if (yaku_function(state, player, branch) > 0) {
        return true;
      }
    }
  }

  return false;
}
}  // namespace mahjong
