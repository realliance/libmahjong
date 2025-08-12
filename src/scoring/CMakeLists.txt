target_sources(
  libmahjong
  PRIVATE "yakus.cc" "scoring.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/src"
         FILES
         "yakus.h"
         "scoring.h")
