cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
project(libmahjong)
include(ProcessorCount)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(PACKAGE_TESTS "Build the tests" OFF)
option(PACKAGE_TOOLS "Build the tools" OFF)
option(USE_CLANG_TIDY "Use Clang Tidy" OFF)
option(CI_CLANG_FORMAT_FLAGS "Use CI Clang Format Flags" OFF)

add_subdirectory(${libmahjong_SOURCE_DIR}/libs)

if(PACKAGE_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(${libmahjong_SOURCE_DIR}/tests)
endif()

add_library(libmahjong SHARED)

set_target_properties(libmahjong PROPERTIES PREFIX "")
set_target_properties(libmahjong PROPERTIES OUTPUT_NAME "libmahjong")

ProcessorCount(N)
if(NOT N EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${N})
  set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${N})
endif()

find_program(clang_format_path NAMES clang-format-10 clang-format)
if (USE_CLANG_TIDY)
  find_program(clang_tidy_path NAMES clang-tidy-10 clang-tidy clang-tidy-11 REQUIRED)
  set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
  )
else()
  find_program(clang_tidy_path NAMES clang-tidy-10 clang-tidy clang-tidy-11)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(libmahjong PRIVATE -pedantic -Wall -Wmost -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(libmahjong PRIVATE -pedantic -Wall -Wextra)
endif()

add_subdirectory(${libmahjong_SOURCE_DIR}/include)
add_subdirectory(${libmahjong_SOURCE_DIR}/controllers)
add_subdirectory(${libmahjong_SOURCE_DIR}/src)

get_target_property(libmahjong_sources libmahjong SOURCES)

if(PACKAGE_TOOLS)
  add_subdirectory(${libmahjong_SOURCE_DIR}/tools)
  get_target_property(driver_sources driver SOURCES)
  list(APPEND libmahjong_sources "${driver_sources}" )
endif()

target_link_libraries(libmahjong PRIVATE ${CMAKE_DL_LIBS})

target_include_directories(libmahjong PUBLIC "${libmahjong_SOURCE_DIR}/include")
target_include_directories(libmahjong PUBLIC "${libmahjong_SOURCE_DIR}/controllers")

add_custom_target(
  check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  VERBATIM
)

find_program(iwyu_path NAMES include-what-you-use iwyu)
if(iwyu_path)
  set_property(TARGET libmahjong PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()

if (clang_tidy_path)
  add_custom_target(
    tidy
    COMMAND clang-tidy
      --quiet --warnings-as-errors=* -p ${libmahjong_BINARY_DIR} ${libmahjong_sources}
    WORKING_DIRECTORY ${libmahjong_SOURCE_DIR}
    VERBATIM
  )
  add_custom_target(
    fix
    COMMAND clang-tidy
      --quiet --fix --fix-errors -p ${libmahjong_BINARY_DIR} ${libmahjong_sources}
    WORKING_DIRECTORY ${libmahjong_SOURCE_DIR}
    VERBATIM
  )
endif()

if (clang_format_path)
  if(CI_CLANG_FORMAT_FLAGS)
    set(CLANG_FORMAT_FLAGS 
      --verbose;
      --Werror;
      -i;
      -n;
    )
  else()
    set(CLANG_FORMAT_FLAGS
      -i;
    )
  endif()

  add_custom_target(
    format
    ALL
    COMMAND ${clang_format_path} ${CLANG_FORMAT_FLAGS}
    ${libmahjong_sources}
    WORKING_DIRECTORY ${libmahjong_SOURCE_DIR}
    VERBATIM
  )

endif()
