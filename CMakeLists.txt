cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 23)
project(libmahjong)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-pedantic -Wall -Wextra -Werror -Wno-unused-parameter)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wmost -Wno-missing-designated-field-initializers
                      -ferror-limit=0)
  if(libmahjong_enable_msan)
    add_compile_options(-fsanitize=memory)
    add_link_options(-fsanitize=memory)
  endif()
  if(libmahjong_enable_asan)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
  endif()
  if(libmahjong_enable_tsan)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
  endif()
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wno-missing-field-initializers)
endif()

include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${N})
  set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${N})
endif()

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(libmahjong_build_tools "whether or not examples should be built" ON)
  option(libmahjong_build_tests "whether or not tests should be built" ON)
  option(libmahjong_use_clang_utils
         "whether or not project is compiled with clang-tidy" ON)
  option(
    libmahjong_use_precompiled_headers
    "whether or not to build with precompiled_headers, incompatible with clang-tidy"
    OFF)
  option(libmahjong_enable_coverage "Enable coverage reporting" OFF)
  option(libmahjong_enable_msan "Enable memory sanitizer" OFF)
  option(libmahjong_enable_asan "Enable address sanitizer" OFF)
  option(libmahjong_enable_tsan "Enable thread sanitizer" OFF)
endif()

# Code Coverage Configuration
add_library(coverage_config INTERFACE)

if(libmahjong_enable_coverage)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC coverage flags
    target_compile_options(coverage_config INTERFACE -O0 -g --coverage)
    target_link_options(coverage_config INTERFACE --coverage)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Clang coverage flags
    target_compile_options(
      coverage_config INTERFACE -O0 -g -fprofile-instr-generate
                                -fcoverage-mapping)
    target_link_options(coverage_config INTERFACE -fprofile-instr-generate
                        -fcoverage-mapping)
  endif()
endif()

if(libmahjong_use_clang_utils AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  find_program(clang_tidy_path NAMES clang-tidy REQUIRED)
  find_program(clang_tidy_cache_path NAMES clang-tidy-cache)

  if(clang_tidy_cache_path)
    set(CMAKE_CXX_CLANG_TIDY "${clang_tidy_cache_path};${clang_tidy_path}")
  else()
    set(CMAKE_CXX_CLANG_TIDY "${clang_tidy_path}")
  endif()

  find_program(clang_apply_replacements_path NAMES clang-apply-replacements)
  if(clang_apply_replacements_path)
    set(CMAKE_CXX_CLANG_TIDY_EXPORT_FIXES_DIR "fixes")
    set(clang_apply_replacements_flags "--style=file"
                                       "--remove-change-desc-files")
    add_custom_target(
      fix
      COMMAND ${clang_apply_replacements_path} ${clang_apply_replacements_flags}
              "${libmahjong_SOURCE_DIR}/build"
      WORKING_DIRECTORY ${libmahjong_SOURCE_DIR}
      VERBATIM)
    set_target_properties(fix PROPERTIES FOLDER code_utils)
  endif()

endif()

add_subdirectory(extern)

add_library(libmahjong SHARED)
add_subdirectory(src)

if(libmahjong_build_tools)
  add_subdirectory(tools)
endif()

if(libmahjong_build_tests)
  enable_testing()
  add_subdirectory(tests)
endif()

find_program(clang_format_path NAMES clang-format)
if(clang_format_path)
  set(clang_format_flags -i)
  get_target_property(libmahjong_sources libmahjong SOURCES)
  get_target_property(libmahjong_includes libmahjong HEADER_SET)
  set(format_files ${libmahjong_sources} ${libmahjong_includes} ${test_sources}
                   ${tool_sources})
  add_custom_target(
    format
    COMMAND ${clang_format_path} ${clang_format_flags} ${format_files}
    WORKING_DIRECTORY ${libmahjong_SOURCE_DIR}
    VERBATIM)
  set_target_properties(format PROPERTIES FOLDER code_utils)
endif()
