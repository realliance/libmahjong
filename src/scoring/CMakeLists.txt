add_subdirectory(yakus)

target_sources(
  libmahjong
  PRIVATE "scoring.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/src"
         FILES
         "yakus.h"
         "scoring.h")
