target_sources(
  libmahjong
  PRIVATE "statecontroller.cc"
          "statecontroller.h"
          "statefunctions.h"
          "stateutilities.cc"
          "stateutilities.h"
          "decisionfunction.cc"
          "decisionfunction.h"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/src"
         FILES
         "statecontroller.h"
         "statefunctions.h"
         "stateutilities.h"
         "decisionfunction.h")

add_subdirectory(gamestates)
