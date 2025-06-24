target_sources(
  libmahjong
  PRIVATE "statecontroller.cc"
          "stateutilities.cc"
          "decisionfunction.cc"
          "router.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/src"
         FILES
         "statecontroller.h"
         "router.h"
         "stateutilities.h"
         "decisionfunction.h")

add_subdirectory(gamestates)
