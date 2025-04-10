#pragma once
#include <vector>

#include "gamestate.h"
#include "handnode.h"
#include "piecetype.h"
#include "score.h"

namespace mahjong {

Score scoreHand(const GameState& state, int player);

int getBasicPoints(Score s);

int getFu(const GameState& state, int player,
          const std::vector<const mahjong::Node*>& branch);

bool isOpenPinfu(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& branch);

int countPiece(const GameState& state, int player, Piece p);

bool isComplete(const GameState& state, int player);

std::vector<Piece> isInTenpai(std::vector<Piece> hand, bool allWaits = false);
std::vector<Piece> isInTenpai13Pieces(std::vector<Piece> hand,
                                      bool allWaits = false);

std::vector<Piece> getRiichiDiscard(std::vector<Piece> hand);

using yakuFunc = int (*)(const GameState&, int,
                         const std::vector<const mahjong::Node*>&);

int isRiichi(const GameState& state, int player,
             const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isFullyConcealedHand(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isPinfu(const GameState& state, int player,
            const std::vector<const mahjong::Node*>& branch);

int isPureDoubleChi(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& branch);

int isAllSimples(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isMixedTripleChi(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& branch);

int isPureStraight(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch);

int isWindOrDragonPon(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& branch);

int isOutsideHand(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& branch);

int isAfterAKan(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isRobbingAKan(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isBottomOfTheSea(const GameState& state, int player,
                     const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isSevenPairs(const GameState& state, int player,
                 const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isTriplePon(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& branch);

int isThreeConcealedPons(const GameState& state, int player,
                         const std::vector<const mahjong::Node*>& branch);

int isThreeKans(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isAllPons(const GameState& state, int player,
              const std::vector<const mahjong::Node*>& branch);

int isHalfFlush(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& branch);

int isLittleThreeDragons(const GameState& state, int player,
                         const std::vector<const mahjong::Node*>& branch);

int isAllTerminalsAndHonors(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isTerminalsInAllSets(const GameState& state, int player,
                         const std::vector<const mahjong::Node*>& branch);

int isTwicePureDoubleChi(const GameState& state, int player,
                         const std::vector<const mahjong::Node*>& branch);

int isBlessingOfMan(const GameState& state, int player,
                    const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isFullFlush(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isThirteenOrphans(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isNineGates(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& branch);

int isBlessingOfHeaven(
    const GameState& state, int player,
    const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isBlessingOfEarth(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isFourConcealedPon(const GameState& state, int player,
                       const std::vector<const mahjong::Node*>& branch);

int isFourKans(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isAllGreen(const GameState& state, int player,
               const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isAllTerminals(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isAllHonors(const GameState& state, int player,
                const std::vector<const mahjong::Node*>& /*unused*/ = {});

int isBigThreeDragons(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& branch);

int isLittleFourWinds(const GameState& state, int player,
                      const std::vector<const mahjong::Node*>& branch);

int isBigFourWinds(const GameState& state, int player,
                   const std::vector<const mahjong::Node*>& branch);

int isMaxBranches(const GameState& state, int player,
                  const std::vector<const mahjong::Node*>& /*unused*/ = {});
}  // namespace mahjong
