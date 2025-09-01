target_sources(
  libmahjong
  PRIVATE "handnode.cc" "handtree.cc" "possiblesets.cc" "util.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/src"
         FILES
         "analysis.h"
         "handnode.h"
         "util.h")
