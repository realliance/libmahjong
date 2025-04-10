include(FetchContent)

if(libmahjong_build_tests)
  FetchContent_Declare(
    googletest
    GIT_TAG main
    GIT_REPOSITORY https://github.com/google/googletest.git)
  FetchContent_MakeAvailable(googletest)
  set_target_properties(gmock PROPERTIES FOLDER libs/gtest)
  set_target_properties(gmock_main PROPERTIES FOLDER libs/gtest)
  set_target_properties(gtest PROPERTIES FOLDER libs/gtest)
  set_target_properties(gtest_main PROPERTIES FOLDER libs/gtest)
  include(GoogleTest)
endif()


macro(package_add_test test_name)
  add_executable(${test_name} ${ARGN})

  target_link_libraries(${test_name} libmahjong GTest::gtest_main)
  target_include_directories(can_chi PRIVATE ${libmahjong_SOURCE_DIR}/src)
  target_compile_options(libmahjong PRIVATE ${lib_mahjong_compile_options})
  set_target_properties(${test_name} PROPERTIES FOLDER tests)
  get_target_property(test_sources ${test_name} SOURCES)
  list(TRANSFORM test_sources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
  set(libmahjong_sources
      ${libmahjong_sources} ${test_sources}
      PARENT_SCOPE)

  gtest_discover_tests(${test_name})
endmacro()

package_add_test(can_chi can_chi.test.cc)
package_add_test(count_pieces count_pieces.test.cc)
package_add_test(remove_pieces remove_pieces.test.cc)
package_add_test(discard_piece discard_piece.test.cc)
package_add_test(piece_type piece_type.test.cc)
package_add_test(handformer handformer.test.cc)
package_add_test(
  yakus
  yakus/riichi.test.cc
  yakus/concealedhand.test.cc
  yakus/pinfu.test.cc
  yakus/puredoublechi.test.cc
  yakus/allsimples.test.cc
  yakus/mixedtriplechi.test.cc
  yakus/purestraight.test.cc
  yakus/dragonpon.test.cc
  yakus/windpons.test.cc
  yakus/outsidehand.test.cc
  yakus/afterkan.test.cc
  yakus/robbingkan.test.cc
  yakus/underthesea.test.cc
  yakus/sevenpairs.test.cc
  yakus/triplepon.test.cc
  yakus/concealedpon.test.cc
  yakus/triplekan.test.cc
  yakus/allpon.test.cc
  yakus/halfflush.test.cc
  yakus/littlethreedragons.test.cc
  yakus/allterminalsandhonors.test.cc
  yakus/terminalinsets.test.cc
  yakus/twicepuredoublechi.test.cc
  yakus/fullflush.test.cc
  yakus/blessingofman.test.cc)
