#pragma once

#include <functional>
#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong {

using yakuFunc = std::function<int(const mahjong::GameState&, int,
                                   const std::vector<const mahjong::Node*>&)>;

enum class YakuId {
  kRiichi,
  kDoubleRiichi,
  kIppatsu,
  kFullyConcealedHand,
  kPinfu,
  kPureDoubleChi,
  kAllSimples,
  kMixedTripleChi,
  kPureStraight,
  kSeatWind,
  kPrevalentWind,
  kWhiteDragon,
  kGreenDragon,
  kRedDragon,
  kOutsideHand,
  kAfterAKan,
  kRobbingAKan,
  kBottomofTheSea,
  kSevenPairs,
  kTriplePon,
  kThreeConcealedPons,
  kThreeKans,
  kAllPons,
  kHalfFlush,
  kLittleThreeDragons,
  kAllTerminalsAndHonors,
  kTerminalsInAllSets,
  kTwicePureDoubleChi,
  kFullFlush,
  kThirteenOrphans,
  kNineGates,
  kBlessingOfHeaven,
  kBlessingofMan,
  kBlessingOfEarth,
  kFourConcealedPon,
  kFourKans,
  kAllGreen,
  kAllTerminals,
  kAllHonors,
  kBigThreeDragons,
  kLittleFourWinds,
  kBigFourWinds,
  kMaxBranches,
};

struct Yaku {  // add yakus for exclusions, name, and enum possibly
  const YakuId id;
  const std::string name;
  const yakuFunc is_yaku_func;
  const std::vector<YakuId> doesnt_score_with;
  const int han;
  const bool bonus_when_closed;
};
}  // namespace mahjong
