set_target_properties(libmahjong PROPERTIES PREFIX "")
set_target_properties(libmahjong PROPERTIES OUTPUT_NAME "libmahjong")

add_subdirectory(controllers)
add_subdirectory(statefunctions)
add_subdirectory(types)
add_subdirectory(analysis)

target_precompile_headers(libmahjong PRIVATE ${project_includes})

install(TARGETS libmahjong DESTINATION lib)
install(TARGETS libmahjong FILE_SET HEADERS)
