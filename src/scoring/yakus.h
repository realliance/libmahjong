#pragma once
#include <vector>

#include "types/gamestate.h"
#include "types/handnode.h"

namespace mahjong {

enum class YakuId {};

bool isRiichi(const GameState& state, int player,
              const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isDoubleRiichi(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isIppatsu(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isFullyConcealedHand(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isPinfu(const GameState& state, int player,
             const std::vector<const mahjong::Node*>& branch);

bool isPureDoubleChi(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& branch);

bool isAllSimples(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isMixedTripleChi(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& branch);

bool isPureStraight(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& branch);

bool isSeatWind(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& branch);
bool isPrevalentWind(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& branch);
bool isWhiteDragon(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch);
bool isGreenDragon(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch);
bool isRedDragon(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch);

bool isOutsideHand(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch);

bool isAfterAKan(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isRobbingAKan(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isBottomOfTheSea(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isSevenPairs(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isTriplePon(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch);

bool isThreeConcealedPons(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch);

bool isThreeKans(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isAllPons(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& branch);

bool isHalfFlush(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch);

bool isLittleThreeDragons(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch);

bool isAllTerminalsAndHonors(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isTerminalsInAllSets(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch);

bool isTwicePureDoubleChi(const GameState& state, int player,
                          const std::vector<const mahjong::Node*>& branch);

bool isBlessingOfMan(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isFullFlush(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isThirteenOrphans(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isNineGates(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch);

bool isBlessingOfHeaven(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isBlessingOfEarth(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isFourConcealedPon(const GameState& state, int player,
                        const std::vector<const mahjong::Node*>& branch);

bool isFourKans(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isAllGreen(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isAllTerminals(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isAllHonors(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*unused*/ = {});

bool isBigThreeDragons(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& branch);

bool isLittleFourWinds(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& branch);

bool isBigFourWinds(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& branch);

bool isMaxBranches(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*unused*/ = {});
}  // namespace mahjong
