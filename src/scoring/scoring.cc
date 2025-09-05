#include "scoring/scoring.h"

#include <algorithm>
#include <vector>

#include "analysis/analysis.h"
#include "analysis/util.h"
#include "scoring/yakus.h"
#include "scoring/yakus/pinfu.h"
#include "scoring/yakus/sevenpairs.h"
#include "scoring/yakus/thirteenorphans.h"
#include "types/gamestate.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/score.h"
#include "types/sets.h"
#include "types/walls.h"

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

}  // namespace

Score scorePlayer(const GameState& state, const Player& player) {
    auto root = breakdownPlayer(player.live_range());
  Score s;
  s.han = 0;
  s.yakuman = 0;
  s.fu = 0;
  if (!root->IsComplete() && !yaku::isThirteenOrphans(state, player)) {
    return s;
  }
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    Score branchscore;
    if (std::ranges::any_of(branch, [](const auto* node) {
          return node->type() == SetType::kSingle;
        })) {
      continue;
    }

    for (const auto& yaku : Yakus::Instance().GetYakus()) {
      if (yaku.type == Yaku::kClosed && player.open) {
        continue;
      }
      if (!yaku.is_yaku_func(state, player, branch)) {
        continue;
      }
      switch (yaku.type) {
        case Yaku::kOpen:
        case Yaku::kClosed:
          branchscore.han += yaku.value;
          break;
        case Yaku::kBonusWhenClosed:
          branchscore.han += yaku.value + (player.open ? 0 : 1);
          break;
        case Yaku::kYakuman:
          branchscore.yakuman++;
          break;
      }
    }

    for (const auto& dora : Walls::GetDoras(state)) {
      for (const auto& p : player.live_range()) {
        if (p == dora) {
          branchscore.han++;
        }
      }
      for (const auto& meld : player.melds_range()) {
        if (meld.start == dora) {
          branchscore.han++;
        }
        if (meld.type == SetType::kChi) {
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

int getFu(const GameState& state, const Player& player,
          const std::vector<const mahjong::Node*>& branch) {
  if (yaku::isSevenPairs(state, player, branch)) {
    return kSevenpairs;
  }
  if (yaku::isPinfu(state, player, branch)) {
    if (player.hasRonned) {
      return kPinfuDiscard;
    }
    return kPinfuSelfdraw;
  }
  int fu = 0;
  if (!player.open && player.hasRonned) {
    fu = kConcealedDiscard;
  } else {
    fu = kOpenOrSelfdraw;
  }
  const bool open = player.open;
  if (isOpenPinfu(state, player, branch)) {
    fu += kOpenpinfu;
  } else if (!player.hasRonned) {
    fu += kSelfdraw;
  }
    for (const auto& meld : player.melds_range()) {
    if (meld.type == SetType::kKan) {
      if (!meld.start.isHonor() && !meld.start.isTerminal()) {
        fu += open ? kSimplekan : kCsimplekan;
      } else {
        fu += open ? kTermHonorkan : kCtermHonorkan;
      }
    }
  }
  for (const auto* node : branch) {
    if (node->type() == SetType::kPon) {
      if (!node->start().isHonor() && !node->start().isTerminal()) {
        fu += open ? kSimplepon : kCsimplepon;
      } else {
        fu += open ? kTermHonorpon : kCtermHonorpon;
      }
    }
    if (open) {
      continue;
    }
    if (node->type() == SetType::kPair) {
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
    if (node->type() == SetType::kChi) {
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

bool isOpenPinfu(const GameState& state, const Player& player,
                 const std::vector<const mahjong::Node*>& branch) {
  for (const auto* node : branch) {
    if (node->type() == SetType::kPon) {
      return false;
    }
    if (node->type() == SetType::kPair) {
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
    for (const auto& meld : player.melds_range()) {
    if (meld.type > SetType::kChi) {
      return false;
    }
  }
  return getWaits(player, state.pendingPiece).size() >= 2;
}

bool isComplete(const GameState& state, const Player& player) {
    auto root = breakdownPlayer(player.live_range());
  if (!root->IsComplete() && !yaku::isThirteenOrphans(state, player)) {
    return false;
  }
  for (const auto& branch : Node::AsBranchVectors(root.get())) {
    if (std::ranges::any_of(branch, [](const auto* node) {
          return node->type() == SetType::kSingle;
        })) {
      continue;
    }
    if (std::ranges::any_of(Yakus::Instance().GetYakus(),
                            [&state, player, &branch](const auto& yaku) {
                              return yaku.is_yaku_func(state, player, branch);
                            })) {
      return true;
    }
  }

  return false;
}
}  // namespace mahjong
