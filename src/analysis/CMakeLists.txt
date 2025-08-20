target_sources(
  libmahjong
  PRIVATE "handtree.cc" "possiblesets.cc" "util.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/src"
         FILES
         "analysis.h"
         "util.h")
