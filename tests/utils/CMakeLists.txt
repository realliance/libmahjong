add_library(test_utils SHARED)
set_target_properties(test_utils PROPERTIES FOLDER tests/utils)
target_link_libraries(test_utils PRIVATE libmahjong)
target_include_directories(test_utils PUBLIC "${libmahjong_SOURCE_DIR}/src")

target_sources(
  test_utils
  PRIVATE "handformer.cc" "playercontrollerfake.cc"
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${libmahjong_SOURCE_DIR}/tests"
         FILES
         "playercontrollerfake.h"
         "handformer.h")
