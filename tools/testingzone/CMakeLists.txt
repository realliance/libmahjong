find_package(PkgConfig REQUIRED)

set(DIR ${CMAKE_CURRENT_LIST_DIR})

add_executable(testingZone "")
target_compile_definitions(testingZone PUBLIC -DNO_PYBIND)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
target_compile_options(LibMahjong PRIVATE -pedantic -Wall -Wmost -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(LibMahjong PRIVATE -pedantic -Wall -Wextra)
endif()
get_target_property(_sources LibMahjong SOURCES)
target_sources(testingZone
    PRIVATE
    "${DIR}/main.cpp"
    ${_sources}
)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(testingZone PRIVATE Threads::Threads)
target_compile_definitions(testingZone PUBLIC -DNO_PYBIND)
target_include_directories(testingZone PRIVATE "${LibMahjong_SOURCE_DIR}/include")
target_include_directories(testingZone PUBLIC "${LibMahjong_SOURCE_DIR}/aicontrollers")

add_custom_command(TARGET testingZone POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:testingZone> ${CMAKE_CURRENT_SOURCE_DIR}/testingZone.tool
)
