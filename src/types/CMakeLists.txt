target_sources(
  libmahjong
  PRIVATE "piecetype.cc" "typeprinter.cc" "walls.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/src"
         FILES
         "event.h"
         "gamestate.h"
         "meld.h"
         "pieces.h"
         "piecetype.h"
         "score.h"
         "settings.h"
         "statefunction.h"
         "typeprinter.h"
         "walls.h"
         "winds.h"
         "yaku.h")
