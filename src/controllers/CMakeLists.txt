target_sources(
  libmahjong
  PRIVATE "angrydiscardobot.cc"
          "thricebot.cc"
          "fasttanyao.cc"
          "alphabeticalbot.cc"
          "totobot.cc"
          "gentlemanbot.cc"
          "controllermanager.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/src"
         FILES
         "playercontroller.h"
         "controllermanager.h")
