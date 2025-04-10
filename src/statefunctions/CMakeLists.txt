target_sources(
  libmahjong
  PRIVATE "statecontroller.cc"
          "statecontroller.h"
          "statefunctions.h"
          "stateutilities.cc"
          "stateutilities.h"
          "decisionfunction.cc"
          "decisionfunction.h")

add_subdirectory(gamestates)
