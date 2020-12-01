cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
project(LibMahjong)

option(PACKAGE_TESTS "Build the tests" OFF)
option(PACKAGE_TOOLS "Build the tools" OFF)

add_subdirectory(${LibMahjong_SOURCE_DIR}/libs)

if(PACKAGE_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(${LibMahjong_SOURCE_DIR}/tests)
endif()

add_library(LibMahjong SHARED)

set_target_properties(LibMahjong PROPERTIES PREFIX "")
set_target_properties(LibMahjong PROPERTIES OUTPUT_NAME "libmahjong")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(LibMahjong PRIVATE -pedantic -Wall -Wmost -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(LibMahjong PRIVATE -pedantic -Wall -Wextra)
endif()

add_subdirectory(${LibMahjong_SOURCE_DIR}/include)
add_subdirectory(${LibMahjong_SOURCE_DIR}/aicontrollers)
add_subdirectory(${LibMahjong_SOURCE_DIR}/src)

if(PACKAGE_TESTS)
  get_target_property(_sources LibMahjong SOURCES)
  add_library(TestLibMahjong ${_sources})
  target_include_directories(TestLibMahjong PUBLIC "${LibMahjong_SOURCE_DIR}/include")
  target_include_directories(TestLibMahjong PUBLIC "${LibMahjong_SOURCE_DIR}/aicontrollers")
endif()

if(PACKAGE_TOOLS)
  add_subdirectory(${LibMahjong_SOURCE_DIR}/tools)
endif()

target_link_libraries(LibMahjong PRIVATE ${CMAKE_DL_LIBS})

target_include_directories(LibMahjong PUBLIC "${LibMahjong_SOURCE_DIR}/include")
target_include_directories(LibMahjong PUBLIC "${LibMahjong_SOURCE_DIR}/aicontrollers")

add_custom_target(
  check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  VERBATIM
)

find_program(iwyu_path NAMES include-what-you-use iwyu)
if(iwyu_path)
  set_property(TARGET LibMahjong PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()
