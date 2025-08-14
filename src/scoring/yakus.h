#pragma once
#include "types/yaku.h"

namespace mahjong {
class Yakus {
 public:
  static const std::vector<Yaku>& GetYakus();
  static void RegisterYaku(Yaku&& yaku);

 private:
  Yakus() = default;
  static Yakus& Instance() {
    static Yakus yakus;
    return yakus;
  }

  std::vector<Yaku> yakus_;
};

#define REGISTER_YAKU(yaku)     \
  namespace {                   \
  Yakus::RegisterYaku(&(yaku)); \
  }

}  // namespace mahjong
