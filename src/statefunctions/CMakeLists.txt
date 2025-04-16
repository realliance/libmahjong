target_sources(
  libmahjong
  PRIVATE "statecontroller.cc"
          "stateutilities.cc"
          "decisionfunction.cc"
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
