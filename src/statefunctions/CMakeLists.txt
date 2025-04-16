target_sources(
  libmahjong
  PRIVATE "statecontroller.cc"
          "statecontroller.h"
          "statefunctions.h"
          "stateutilities.cc"
          "stateutilities.h"
          "decisionfunction.cc"
          "decisionfunction.h")
target_precompile_headers(
  libmahjong PRIVATE "statecontroller.h" "statefunctions.h" "stateutilities.h"
  "decisionfunction.h")

add_subdirectory(gamestates)
