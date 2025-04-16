cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 20)
project(libmahjong)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-pedantic -Wall -Wextra -Werror -Wno-unused-parameter)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wmost -Wno-missing-designated-field-initializers)
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

add_library(libmahjong SHARED)
add_subdirectory(src)

if(libmahjong_build_tools)
  add_subdirectory(tools)
endif()

if(libmahjong_build_tests)
  enable_testing()
  add_subdirectory(tests)
endif()

if(libmahjong_use_clang_utils)
  find_program(clang_format_path NAMES clang-format)
  if(clang_format_path)
    set(clang_format_flags -i)
    set(format_files ${clang_format_flags} ${project_sources}
                     ${project_includes} ${test_sources} ${tool_sources})
    add_custom_target(
      format ALL
      COMMAND ${clang_format_path} ${format_files}
      WORKING_DIRECTORY ${libmahjong_SOURCE_DIR}
      VERBATIM)
    set_target_properties(format PROPERTIES FOLDER code_utils)
  endif()
endif()
