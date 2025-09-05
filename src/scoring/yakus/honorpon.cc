#include "scoring/yakus/honorpon.h"

#include <vector>

#include "analysis/handnode.h"
#include "scoring/yakus.h"
#include "statefunctions/stateutilities.h"
#include "types/gamestate.h"
#include "types/hand.h"
#include "types/pieces.h"
#include "types/piecetype.h"
#include "types/sets.h"
#include "types/winds.h"
#include "types/yaku.h"

namespace mahjong::yaku {
namespace {
bool findPon(const GameState& /* state */, const Hand& player,
             const std::vector<const mahjong::Node*>& branch,
             const Piece& piece) {
  for (const auto* node : branch) {
    if (node->type() == SetType::kPon && node->start() == piece) {
      return true;
    }
  }
  for (const auto& meld : player.melds_range()) {
    if (meld.type >= SetType::kPon && meld.start == piece) {
      return true;
    }
  }
  return false;
}

}  // namespace

bool isSeatWind(const GameState& state, const Hand& player,
                const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch,
                 Piece::fromWind(GetSeat(state.roundNum, player.id)));
}

bool isPrevalentWind(const GameState& state, const Hand& player,
                     const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch,
                 Piece::fromWind(state.roundNum > 3 ? kSouth : kEast));
}

bool isGreenDragon(const GameState& state, const Hand& player,
                   const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch, kGreenDragon);
}

bool isRedDragon(const GameState& state, const Hand& player,
                 const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch, kRedDragon);
}

bool isWhiteDragon(const GameState& state, const Hand& player,
                   const std::vector<const mahjong::Node*>& branch) {
  return findPon(state, player, branch, kWhiteDragon);
}

REGISTER_YAKUS({
    {
        .id = "seatwind",
        .name = "Seat Wind Pon",
        .type = Yaku::kOpen,
        .value = 1,
        .is_yaku_func = yaku::isSeatWind,
    },
    {
        .id = "prevalentwind",
        .name = "Prevalent Wind Pon",
        .type = Yaku::kOpen,
        .value = 1,
        .is_yaku_func = yaku::isPrevalentWind,
    },
    {
        .id = "greendragon",
        .name = "Green Dragon Pon",
        .type = Yaku::kOpen,
        .value = 1,
        .is_yaku_func = yaku::isGreenDragon,
    },
    {
        .id = "reddragon",
        .name = "Red Dragon Pon",
        .type = Yaku::kOpen,
        .value = 1,
        .is_yaku_func = yaku::isRedDragon,
    },
    {
        .id = "whitedragon",
        .name = "White Dragon Pon",
        .type = Yaku::kOpen,
        .value = 1,
        .is_yaku_func = yaku::isWhiteDragon,
    },
});
}  // namespace mahjong::yaku
