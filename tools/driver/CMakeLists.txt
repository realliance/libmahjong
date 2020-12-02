find_package(PkgConfig REQUIRED)

set(DIR ${CMAKE_CURRENT_LIST_DIR})

add_executable(driver "")
target_compile_definitions(driver PUBLIC -DNO_PYBIND)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
target_compile_options(libmahjong PRIVATE -pedantic -Wall -Wmost -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(libmahjong PRIVATE -pedantic -Wall -Wextra)
endif()
get_target_property(_sources libmahjong SOURCES)
target_sources(driver
    PRIVATE
    "${DIR}/main.cpp"
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(driver PRIVATE Threads::Threads libmahjong)
target_include_directories(driver PRIVATE "${libmahjong_SOURCE_DIR}/include")
target_include_directories(driver PUBLIC "${libmahjong_SOURCE_DIR}/controllers")

add_custom_command(TARGET driver POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:driver> ${CMAKE_CURRENT_SOURCE_DIR}/driver.tool
)
