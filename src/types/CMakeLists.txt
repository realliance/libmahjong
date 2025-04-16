target_sources(libmahjong PRIVATE "handnode.cc" "piecetype.cc" "typeprinter.cc"
                                  "walls.cc")
target_precompile_headers(
  libmahjong
  PRIVATE
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
