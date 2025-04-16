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
         .
         FILES
         "angrydiscardobot.h"
         "thricebot.h"
         "fasttanyao.h"
         "alphabeticalbot.h"
         "totobot.h"
         "gentlemanbot.h"
         "playercontroller.h"
         "controllermanager.h")
