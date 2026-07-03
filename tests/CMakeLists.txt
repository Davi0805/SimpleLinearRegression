include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(linear_regression_tests test_main.cpp)
target_include_directories(linear_regression_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(linear_regression_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(linear_regression_tests)
