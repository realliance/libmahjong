target_sources(libmahjong PRIVATE  "handtree.cc" "possiblesets.cc" "hands.cc" )
target_precompile_headers(
  libmahjong
  PRIVATE
  "hands.h"
"analysis.h")
