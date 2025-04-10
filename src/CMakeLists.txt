

add_subdirectory(controllers)
add_subdirectory(statefunctions)

target_sources(
  libmahjong
  PRIVATE "analysis.h"
          "controllermanager.cc"
          "controllermanager.h"
          "decisionfunction.cc"
          "decisionfunction.h"
          "event.h"
          "gamestate.h"
          "hand.h"
          "handformer.cc"
          "handformer.h"
          "handnode.cc"
          "handnode.h"
          "hands.cc"
          "hands.h"
          "handtree.cc"
          "meld.h"
          "pieces.h"
          "piecetype.cc"
          "piecetype.h"
          "player.h"
          "playercontroller.h"
          "possiblesets.cc"
          "score.h"
          "settings.h"
          "statecontroller.cc"
          "statefunctions.h"
          "stateutilities.cc"
          "stateutilities.h"
          "typeprinter.cc"
          "typeprinter.h"
          "walls.cc"
          "walls.h"
          "winds.h")
get_target_property(libmahjong_sources_ libmahjong SOURCES)
set(libmahjong_sources
    ${libmahjong_sources} ${libmahjong_sources_}
    PARENT_SCOPE)
