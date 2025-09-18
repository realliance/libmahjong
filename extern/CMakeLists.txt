include(FetchContent)
set(CMAKE_CXX_CLANG_TIDY "")

# Running -fsanitize=msan with clang requires we build gtest with it
# instrumented.
if(libmahjong_build_tests AND libmahjong_enable_msan)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    GIT_SHALLOW)
  FetchContent_MakeAvailable(googletest)
endif()
