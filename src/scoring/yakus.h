#pragma once
#include <array>
#include "types/yaku.h"

#include "scoring/yakus/afterakan.h"
#include "scoring/yakus/allgreen.h"
#include "scoring/yakus/allhonors.h"
#include "scoring/yakus/allpons.h"
#include "scoring/yakus/allsimples.h"
#include "scoring/yakus/allterminals.h"
#include "scoring/yakus/allterminalsandhonors.h"
#include "scoring/yakus/bigfourwinds.h"
#include "scoring/yakus/bigthreedragons.h"
#include "scoring/yakus/blessingofearth.h"
#include "scoring/yakus/blessingofheaven.h"
#include "scoring/yakus/blessingofman.h"
#include "scoring/yakus/bottomofthesea.h"
#include "scoring/yakus/fourconcealedpon.h"
#include "scoring/yakus/fourkans.h"
#include "scoring/yakus/fullflush.h"
#include "scoring/yakus/fullyconcealedhand.h"
#include "scoring/yakus/halfflush.h"
#include "scoring/yakus/honorpon.h"
#include "scoring/yakus/littlefourwinds.h"
#include "scoring/yakus/littlethreedragons.h"
#include "scoring/yakus/mixedtriplechi.h"
#include "scoring/yakus/ninegates.h"
#include "scoring/yakus/outsidehand.h"
#include "scoring/yakus/pinfu.h"
#include "scoring/yakus/puredoublechi.h"
#include "scoring/yakus/purestraight.h"
#include "scoring/yakus/robbingakan.h"
#include "scoring/yakus/sevenpairs.h"
#include "scoring/yakus/terminalsinallsets.h"
#include "scoring/yakus/thirteenorphans.h"
#include "scoring/yakus/threeconcealedpons.h"
#include "scoring/yakus/threekans.h"
#include "scoring/yakus/triplepon.h"
#include "scoring/yakus/twicepuredoublechi.h"

namespace mahjong::yakus {
constexpr int kNumYakus = 27;
const std::array<yakuFunc, kNumYakus> kYakuFunctions{
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
constexpr int kNumYakuman = 12;
const std::array<yakuFunc, kNumYakuman> kYakumanFunctions = {
    isThirteenOrphans, isNineGates,        isBlessingOfHeaven,
    isBlessingOfEarth, isFourConcealedPon, isFourKans,
    isAllGreen,        isAllTerminals,     isAllHonors,
    isBigThreeDragons, isLittleFourWinds,  isBigFourWinds,
};

}  // namespace mahjong::yakus
