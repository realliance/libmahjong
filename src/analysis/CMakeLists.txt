target_sources(
  libmahjong
  PRIVATE "handtree.cc" "possiblesets.cc" "hands.cc"
  PUBLIC FILE_SET HEADERS BASE_DIRS . FILES "hands.h" "analysis.h")
