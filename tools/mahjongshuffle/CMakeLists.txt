find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(DIR ${CMAKE_CURRENT_LIST_DIR})

add_executable(mahjongShuffle "")
target_compile_definitions(mahjongShuffle PUBLIC -DNO_PYBIND)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(mahjongShuffle PRIVATE -Wall -Wmost -Wextra -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(mahjongShuffle PRIVATE -Wall -Wextra)
endif()

target_sources(mahjongShuffle
    PRIVATE
    "${DIR}/main.cpp"
    "${DIR}/mainwindow.cpp"
    "${DIR}/mainwindow.h"
    "${LibMahjong_SOURCE_DIR}/src/piecetype.cpp"
    "${LibMahjong_SOURCE_DIR}/src/walls.cpp"
    "${LibMahjong_SOURCE_DIR}/src/possiblesets.cpp"
    "${LibMahjong_SOURCE_DIR}/src/handtree.cpp"
    "${LibMahjong_SOURCE_DIR}/src/handnode.cpp"
)
target_link_libraries(mahjongShuffle ${GTKMM_LIBRARIES})
target_include_directories(mahjongShuffle PRIVATE "${LibMahjong_SOURCE_DIR}/include")

add_custom_command(TARGET mahjongShuffle POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mahjongShuffle> ${CMAKE_CURRENT_SOURCE_DIR}/mahjongshuffle.tool
)
