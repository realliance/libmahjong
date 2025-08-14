#include "scoring/yakus.h"
#include <vector>
#include "types/yaku.h"

namespace mahjong {
const std::vector<Yaku>& Yakus::GetYakus() {
  return Instance().yakus_;
}
bool Yakus::RegisterYaku(const Yaku& yaku) {
  Instance().yakus_.push_back(yaku);
  return true;
}
bool Yakus::RegisterYakus(const std::vector<Yaku>& yakus) {
  for (const auto& yaku : yakus) {
    Instance().yakus_.push_back(yaku);
  }
  return true;
}
}  // namespace mahjong
