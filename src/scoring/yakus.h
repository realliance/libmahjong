#pragma once
#include "types/yaku.h"

namespace mahjong {
class Yakus {
 public:
  static Yakus& Instance() {
    static Yakus yakus;
    return yakus;
  }

  const std::vector<Yaku>& GetYakus();
  bool RegisterYaku(const Yaku& yaku);
  bool RegisterYakus(const std::vector<Yaku>& yakus);

 private:
  Yakus() = default;

  std::vector<Yaku> yakus_;
};

#define REGISTER_YAKU(...)                                                \
  namespace {                                                             \
  bool __registered = Yakus::Instance().RegisterYaku((Yaku __VA_ARGS__)); \
  }

#define REGISTER_YAKUS(...)                                            \
  namespace {                                                          \
  bool __registered =                                                  \
      Yakus::Instance().RegisterYakus(std::vector<Yaku>(__VA_ARGS__)); \
  }

}  // namespace mahjong
