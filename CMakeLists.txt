cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 20)
project(libmahjong)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(clang_tidy_path)
  set(CMAKE_CXX_CLANG_TIDY "${clang_tidy_path}")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(libmahjong_compile_options -pedantic -Wall -Wmost -Wextra -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(libmahjong_compile_options -pedantic -Wall -Wextra -Werror)
endif()

include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${N})
  set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${N})
endif()

add_library(libmahjong SHARED)
set_target_properties(libmahjong PROPERTIES PREFIX "")
set_target_properties(libmahjong PROPERTIES OUTPUT_NAME "libmahjong")

target_compile_options(libmahjong PRIVATE ${lib_mahjong_compile_options})
target_link_libraries(libmahjong PRIVATE ${CMAKE_DL_LIBS})
target_include_directories(libmahjong PUBLIC "${libmahjong_SOURCE_DIR}/src")

add_subdirectory(src)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(libmahjong_build_tools "whether or not examples should be built" ON)
  option(libmahjong_build_tests "whether or not tests should be built" ON)
  option(libmahjong_clang_tidy
         "whether or not project is compiled with clang-tidy" ON)

  if(libmahjong_build_tools)
    add_subdirectory(tools)
  endif()

  if(libmahjong_build_tests)
    enable_testing()
    add_subdirectory(tests)
  endif()

  if(libmahjong_clang_tidy)
    find_program(clang_tidy_path NAMES clang-tidy REQUIRED)
    set_target_properties(libmahjong PROPERTIES CXX_CLANG_TIDY
                                                "${clang_tidy_path}")
    add_custom_target(
      fix
      COMMAND clang-tidy --fix-errors -p=build ${project_sources}
      WORKING_DIRECTORY ${libmahjong_SOURCE_DIR}
      VERBATIM)
    set_target_properties(fix PROPERTIES FOLDER code_utils)
    add_custom_target(
      tidy
      COMMAND clang-tidy -p=build ${project_sources}
      WORKING_DIRECTORY ${libmahjong_SOURCE_DIR}
      VERBATIM)
    set_target_properties(tidy PROPERTIES FOLDER code_utils)
  endif()

  find_program(iwyu_path NAMES include-what-you-use iwyu)
  if(iwyu_path)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  endif()

  find_program(iwyu_tool_path NAMES iwyu-tool)
  find_program(iwyu_fix_includes NAMES iwyu-fix-includes)
  if(iwyu_tool_path AND iwyu_fix_includes)
    add_custom_target(
      fix_includes
      COMMAND ${CMAKE_SOURCE_DIR}/iwyu_fix.sh
      WORKING_DIRECTORY ${libmahjong_SOURCE_DIR}
      VERBATIM)
    set_target_properties(fix_includes PROPERTIES FOLDER code_utils)
  endif()

  find_program(clang_format_path NAMES clang-format)
  if(clang_format_path)
    set(CLANG_FORMAT_FLAGS -i;)
    add_custom_target(
      format ALL
      COMMAND ${clang_format_path} ${CLANG_FORMAT_FLAGS} ${project_sources}
      WORKING_DIRECTORY ${libmahjong_SOURCE_DIR}
      VERBATIM)
    set_target_properties(format PROPERTIES FOLDER code_utils)
  endif()

endif()
