#pragma once
#include <vector>

#include "piecetype.h"
#include "score.h"
namespace Mahjong {
class Node;
}

namespace Mahjong {
struct GameState;

auto scoreHand(const GameState& state, int player) -> Score;

auto getBasicPoints(Score s) -> int;

auto getFu(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isOpenPinfu(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> bool;

auto countPiece(const GameState& state, int player, Piece p) -> int;

auto isComplete(const GameState& state, int player) -> bool;

auto isInTenpai(std::vector<Piece> hand, bool allWaits = false) -> std::vector<Piece>;
auto isInTenpai13Pieces(std::vector<Piece> hand, bool allWaits = false) -> std::vector<Piece>;

auto getRiichiDiscard(std::vector<Piece> hand) -> std::vector<Piece>;

using yakuFunc = int (*)(const GameState&, int, const std::vector<const Mahjong::Node*>&);

auto isRiichi(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isFullyConcealedHand(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isPinfu(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isPureDoubleChi(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isAllSimples(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isMixedTripleChi(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isPureStraight(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isWindOrDragonPon(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isOutsideHand(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isAfterAKan(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isRobbingAKan(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isBottomOfTheSea(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isSevenPairs(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isTriplePon(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isThreeConcealedPons(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isThreeKans(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isAllPons(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isHalfFlush(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isLittleThreeDragons(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isAllTerminalsAndHonors(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isTerminalsInAllSets(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isTwicePureDoubleChi(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isBlessingOfMan(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isFullFlush(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isThirteenOrphans(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isNineGates(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isBlessingOfHeaven(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isBlessingOfEarth(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isFourConcealedPon(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isFourKans(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isAllGreen(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isAllTerminals(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isAllHonors(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;

auto isBigThreeDragons(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isLittleFourWinds(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isBigFourWinds(const GameState& state, int player, const std::vector<const Mahjong::Node*>& branch) -> int;

auto isMaxBranches(const GameState& state, int player, const std::vector<const Mahjong::Node*>& /*unused*/ = {}) -> int;
}  // namespace Mahjong
