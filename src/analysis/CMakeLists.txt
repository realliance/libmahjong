target_sources(
  libmahjong
  PRIVATE "handtree.cc" "possiblesets.cc" "yakus.cc" "util.cc" "scoring.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/src"
         FILES
         "yakus.h"
         "analysis.h"
         "util.h"
         "scoring.h")
