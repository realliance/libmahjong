

add_subdirectory(controllers)
add_subdirectory(statefunctions)

target_sources(
  libmahjong
  PRIVATE "analysis.h"
          "controllermanager.cc"
          "controllermanager.h"
          "decisionfunction.cc"
          "decisionfunction.h"
          "event.cc"
          "event.h"
          "gamestate.cc"
          "gamestate.h"
          "hand.cc"
          "hand.h"
          "handformer.cc"
          "handformer.h"
          "handnode.cc"
          "handnode.h"
          "hands.cc"
          "hands.h"
          "handtree.cc"
          "meld.cc"
          "meld.h"
          "pieces.h"
          "piecetype.cc"
          "piecetype.h"
          "player.cc"
          "player.h"
          "playercontroller.h"
          "possiblesets.cc"
          "score.h"
          "settings.h"
          "statecontroller.cc"
          "statefunctions.h"
          "stateutilities.cc"
          "stateutilities.h"
          "walls.cc"
          "walls.h"
          "winds.h")
get_target_property(libmahjong_sources_ libmahjong SOURCES)
set(libmahjong_sources
    ${libmahjong_sources} ${libmahjong_sources_}
    PARENT_SCOPE)
