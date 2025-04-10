find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(driver "")
set_target_properties(driver PROPERTIES FOLDER tools)
target_compile_options(driver PRIVATE ${lib_mahjong_compile_options})

target_include_directories(driver PRIVATE "${libmahjong_SOURCE_DIR}/src")
target_link_libraries(driver PRIVATE Threads::Threads libmahjong)

target_sources(driver PRIVATE "main.cc")

get_target_property(driver_sources driver SOURCES)
list(TRANSFORM driver_sources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
set(tool_sources
    ${tool_sources} ${driver_sources}
    PARENT_SCOPE)
