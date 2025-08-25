set_target_properties(libmahjong PROPERTIES PREFIX "")
set_target_properties(libmahjong PROPERTIES OUTPUT_NAME "libmahjong")

add_subdirectory(api)
add_subdirectory(controllers)
add_subdirectory(statefunctions)
add_subdirectory(types)
add_subdirectory(analysis)
add_subdirectory(scoring)

get_target_property(libmahjong_includes libmahjong HEADER_SET)
if(libmahjong_use_precompiled_headers)
  target_precompile_headers(libmahjong PRIVATE ${libmahjong_includes})
endif()

if(libmahjong_enable_coverage)
  target_link_libraries(libmahjong PUBLIC coverage_config)
endif()

install(TARGETS libmahjong DESTINATION lib)
install(TARGETS libmahjong FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libmahjong)
