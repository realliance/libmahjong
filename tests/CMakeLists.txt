include(CTest)
find_package(GTest REQUIRED)

function(package_add_test test_name)
  add_executable(${test_name})
  target_sources(${test_name} PRIVATE ${ARGN})

  target_link_libraries(${test_name} libmahjong test_utils GTest::gtest_main)
  target_include_directories(
    ${test_name} PRIVATE ${libmahjong_SOURCE_DIR}/src
                         ${libmahjong_SOURCE_DIR}/tests)
  if(libmahjong_use_precompiled_headers)
    target_precompile_headers(${test_name} REUSE_FROM test_precompiled_headers)
  endif()
  set_target_properties(${test_name} PROPERTIES FOLDER tests/${test_name})
  gtest_discover_tests(${test_name})

  get_target_property(test_source ${test_name} SOURCES)
  list(TRANSFORM test_source PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
  set(test_sources
      ${test_sources} ${test_source}
      PARENT_SCOPE)
endfunction()

if(libmahjong_use_precompiled_headers)
  add_library(test_precompiled_headers STATIC)
  get_target_property(libmahjong_includes libmahjong HEADER_SET)
  target_precompile_headers(test_precompiled_headers PRIVATE
                            ${libmahjong_includes})
  file(
    GENERATE
    OUTPUT empty_pch.cc
    CONTENT "/*dummy pch file*/")
  target_sources(test_precompiled_headers PRIVATE empty_pch.cc)
  target_include_directories(test_precompiled_headers
                             PRIVATE ${libmahjong_SOURCE_DIR}/src)
  set_target_properties(test_precompiled_headers PROPERTIES FOLDER tests)
endif()

add_subdirectory(utils)

package_add_test(can_chi "can_chi.test.cc")
package_add_test(count_pieces "count_pieces.test.cc")
package_add_test(discard_piece "discard_piece.test.cc")
package_add_test(game_play "game_play.test.cc")
package_add_test(piece_type "piece_type.test.cc")
package_add_test(remove_pieces "remove_pieces.test.cc")
package_add_test(setup "setup.test.cc")

package_add_test(handformer "utils/handformer.test.cc")
package_add_test(gamestate_utils "utils/gamestate_utils.test.cc")

package_add_test(
  yakus
  "yakus/riichi.test.cc"
  "yakus/concealedhand.test.cc"
  "yakus/pinfu.test.cc"
  "yakus/puredoublechi.test.cc"
  "yakus/allsimples.test.cc"
  "yakus/mixedtriplechi.test.cc"
  "yakus/purestraight.test.cc"
  "yakus/dragonpon.test.cc"
  "yakus/windpons.test.cc"
  "yakus/outsidehand.test.cc"
  "yakus/afterkan.test.cc"
  "yakus/robbingkan.test.cc"
  "yakus/underthesea.test.cc"
  "yakus/sevenpairs.test.cc"
  "yakus/triplepon.test.cc"
  "yakus/concealedpon.test.cc"
  "yakus/triplekan.test.cc"
  "yakus/allpon.test.cc"
  "yakus/halfflush.test.cc"
  "yakus/littlethreedragons.test.cc"
  "yakus/allterminalsandhonors.test.cc"
  "yakus/terminalinsets.test.cc"
  "yakus/twicepuredoublechi.test.cc"
  "yakus/fullflush.test.cc"
  "yakus/blessingofman.test.cc")

package_add_test(
  rules
  "rules/basic_rules.test.cc"
  "rules/calls.test.cc"
)

get_target_property(test_utils_sources test_utils SOURCES)
list(TRANSFORM test_utils_sources PREPEND ${CMAKE_CURRENT_LIST_DIR}/utils/)
get_target_property(test_utils_includes test_utils HEADER_SET)
set(test_sources
    ${test_sources} ${test_utils_sources} ${test_utils_includes}
    PARENT_SCOPE)
