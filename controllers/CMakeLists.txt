set(DIR ${CMAKE_CURRENT_LIST_DIR})
target_sources(libmahjong
  PRIVATE
    "${DIR}/angrydiscardobot.cpp"
    "${DIR}/angrydiscardobot.h"
    "${DIR}/thricebot.h"
    "${DIR}/thricebot.cpp"
    "${DIR}/fasttanyao.cpp"
    "${DIR}/fasttanyao.h"
    "${DIR}/alphabeticalbot.cpp"
    "${DIR}/alphabeticalbot.h"
    "${DIR}/totobot.cpp"
    "${DIR}/totobot.h"
    "${DIR}/gentlemanbot.cpp"
    "${DIR}/gentlemanbot.h"
)
