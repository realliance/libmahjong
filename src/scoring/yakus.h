#pragma once
#include "types/yaku.h"

namespace mahjong {
class Yakus {
 public:
  static const std::vector<Yaku>& GetYakus();
  static bool RegisterYaku(const Yaku& yaku);
  static bool RegisterYakus(const std::vector<Yaku>& yakus);

 private:
  Yakus() = default;
  static Yakus& Instance() {
    static Yakus yakus;
    return yakus;
  }

  std::vector<Yaku> yakus_;
};

#define REGISTER_YAKU(...)                                     \
  namespace {                                                  \
  bool __registered = Yakus::RegisterYaku((Yaku __VA_ARGS__)); \
  }

#define REGISTER_YAKUS(...)                                                 \
  namespace {                                                               \
  bool __registered = Yakus::RegisterYakus(std::vector<Yaku>(__VA_ARGS__)); \
  }

}  // namespace mahjong
