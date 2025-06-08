target_sources(libmahjong PRIVATE
               "gamestate.cc" PUBLIC FILE_SET HEADERS BASE_DIRS
               "${libmahjong_SOURCE_DIR}/src" FILES
               "gamestate.h"
               "types.h")
