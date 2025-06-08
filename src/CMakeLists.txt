set_target_properties(libmahjong PROPERTIES PREFIX "")
set_target_properties(libmahjong PROPERTIES OUTPUT_NAME "libmahjong")

add_subdirectory(api)
add_subdirectory(controllers)
add_subdirectory(statefunctions)
add_subdirectory(types)
add_subdirectory(analysis)

get_target_property(libmahjong_includes libmahjong HEADER_SET)
target_precompile_headers(libmahjong PRIVATE ${libmahjong_includes})

install(TARGETS libmahjong DESTINATION lib)
install(TARGETS libmahjong FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libmahjong)
