target_sources(
  libmahjong
  PRIVATE "analysis.cc" "handnode.cc" "handtree.cc" "util.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/src"
         FILES
         "analysis.h"
         "handnode.h"
         "handtree.h"
         "util.h")
