mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

# https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html
macro(package_add_test TESTNAME)
    # create an exectuable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} gtest gmock gtest_main libmahjong )
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${TESTNAME} PRIVATE -pedantic -Wall -Wmost -Wextra)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${TESTNAME} PRIVATE -pedantic -Wall -Wextra)
    endif()
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
    target_compile_definitions(${TESTNAME} PUBLIC -DNO_PYBIND)
endmacro()

# ADD TESTS BELOW #
package_add_test(Can_Chi can_chi.test.cpp)
package_add_test(Count_Pieces count_pieces.test.cpp)
package_add_test(Remove_Pieces remove_pieces.test.cpp)
package_add_test(Discard_Piece discard_piece.test.cpp)
package_add_test(Piece_Type piece_type.test.cpp)
package_add_test(HandFormer handformer.test.cpp)
package_add_test(Yakus 
    yakus/riichi.test.cpp 
    yakus/concealedhand.test.cpp
    yakus/pinfu.test.cpp
    yakus/puredoublechi.test.cpp
    yakus/allsimples.test.cpp
    yakus/mixedtriplechi.test.cpp
    yakus/purestraight.test.cpp
    yakus/dragonpon.test.cpp
    yakus/windpons.test.cpp
    yakus/outsidehand.test.cpp
    yakus/afterkan.test.cpp
    yakus/robbingkan.test.cpp
    yakus/underthesea.test.cpp
    yakus/sevenpairs.test.cpp
    yakus/triplepon.test.cpp
    yakus/concealedpon.test.cpp
    yakus/triplekan.test.cpp
    yakus/allpon.test.cpp
    yakus/halfflush.test.cpp
    yakus/littlethreedragons.test.cpp
    yakus/allterminalsandhonors.test.cpp
    yakus/terminalinsets.test.cpp
    yakus/twicepuredoublechi.test.cpp
    yakus/fullflush.test.cpp
    yakus/blessingofman.test.cpp
    yakus/thirteenorphans.test.cpp
    yakus/ninegates.test.cpp
    yakus/blessingofheaven.test.cpp
    yakus/blessingofearth.test.cpp
    yakus/fourconcealedpons.test.cpp
    yakus/fourkans.test.cpp
    yakus/allgreen.test.cpp
    yakus/allterminals.test.cpp
    yakus/allhonors.test.cpp
    yakus/bigthreedragons.test.cpp
    yakus/littlefourwinds.test.cpp
    yakus/bigfourwinds.test.cpp
    yakus/maxbranches.test.cpp
)
