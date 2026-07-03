cmake_minimum_required(VERSION 3.14)
project(linear_regression CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    rapidcsv
    GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
    GIT_TAG        v8.83
)
FetchContent_MakeAvailable(rapidcsv)

add_executable(linear_regression src/main.cpp)
target_include_directories(linear_regression PRIVATE include)
target_link_libraries(linear_regression PRIVATE rapidcsv)

# `cmake --build . --target run` (or `make run`): build then execute the binary
add_custom_target(run
    COMMAND linear_regression
    DEPENDS linear_regression
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running linear_regression"
    USES_TERMINAL
)

enable_testing()
add_subdirectory(tests)
