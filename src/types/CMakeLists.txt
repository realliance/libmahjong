target_sources(
  libmahjong
  PRIVATE "handnode.cc" "piecetype.cc" "typeprinter.cc" "walls.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         .
         FILES
         "event.h"
         "gamestate.h"
         "hand.h"
         "handnode.h"
         "meld.h"
         "pieces.h"
         "piecetype.h"
         "player.h"
         "score.h"
         "settings.h"
         "typeprinter.h"
         "walls.h"
         "winds.h")
