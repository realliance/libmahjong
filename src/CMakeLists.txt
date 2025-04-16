add_subdirectory(controllers)
add_subdirectory(statefunctions)
add_subdirectory(types)
add_subdirectory(analysis)

get_target_property(libmahjong_sources libmahjong SOURCES)
set(project_sources
    ${project_sources} ${libmahjong_sources}
    PARENT_SCOPE)
get_target_property(libmahjong_includes libmahjong PRECOMPILE_HEADERS)
set(project_includes
    ${project_includes} ${libmahjong_includes}
    PARENT_SCOPE)
