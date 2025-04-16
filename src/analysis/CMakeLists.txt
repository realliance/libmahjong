target_sources(
  libmahjong
  PRIVATE "handtree.cc" "possiblesets.cc" "hands.cc"
  PUBLIC FILE_SET HEADERS BASE_DIRS "${libmahjong_SOURCE_DIR}/src" FILES "hands.h" "analysis.h")
