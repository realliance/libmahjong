add_subdirectory(controllers)
add_subdirectory(statefunctions)
add_subdirectory(types)
add_subdirectory(analysis)

get_target_property(libmahjong_sources libmahjong SOURCES)
set(project_sources
    ${project_sources} ${libmahjong_sources}
    PARENT_SCOPE)
