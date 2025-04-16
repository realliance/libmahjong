set_target_properties(libmahjong PROPERTIES PREFIX "")
set_target_properties(libmahjong PROPERTIES OUTPUT_NAME "libmahjong")

target_include_directories(libmahjong PUBLIC "${libmahjong_SOURCE_DIR}/src")

add_subdirectory(controllers)
add_subdirectory(statefunctions)
add_subdirectory(types)
add_subdirectory(analysis)

get_target_property(libmahjong_sources libmahjong SOURCES)
set(project_sources
    ${project_sources} ${libmahjong_sources}
    PARENT_SCOPE)
get_target_property(libmahjong_includes libmahjong HEADER_SET)
set(project_includes
    ${project_includes} ${libmahjong_includes}
    PARENT_SCOPE)

target_precompile_headers(libmahjong PRIVATE ${project_includes})

install(TARGETS libmahjong DESTINATION lib)
install(TARGETS libmahjong FILE_SET HEADERS)
