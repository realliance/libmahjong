add_library(test_utils SHARED)
set_target_properties(test_utils PROPERTIES PREFIX "")
set_target_properties(test_utils PROPERTIES OUTPUT_NAME "test_utils")
set_target_properties(test_utils PROPERTIES FOLDER tests/utils)

target_compile_options(test_utils PRIVATE ${lib_mahjong_compile_options})
target_link_libraries(test_utils PRIVATE libmahjong)
target_include_directories(test_utils PUBLIC "${libmahjong_SOURCE_DIR}/src")
target_sources(test_utils PRIVATE "handformer.cc" "handformer.h" "testai.cc"
                                  "testai.h")

get_target_property(test_utils_sources test_utils SOURCES)
list(TRANSFORM test_utils_sources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
set(libmahjong_sources
    ${libmahjong_sources} ${test_utils_sources}
    PARENT_SCOPE)
