#include "scoring/yakus.h"
#include <vector>
#include "types/yaku.h"

namespace mahjong {
const std::vector<Yaku>& Yakus::GetYakus() {
  return Instance().yakus_;
}
void Yakus::RegisterYaku(Yaku&& yaku) {
  return Instance().yakus_.push_back(yaku);
}
}  // namespace mahjong
