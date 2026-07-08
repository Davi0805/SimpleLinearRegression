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

find_package(Boost REQUIRED)

FetchContent_Declare(
    boost_program_options
    GIT_REPOSITORY https://github.com/boostorg/program_options.git
    GIT_TAG        boost-1.74.0
)
FetchContent_MakeAvailable(boost_program_options)

add_library(boost_program_options_lib STATIC
    ${boost_program_options_SOURCE_DIR}/src/cmdline.cpp
    ${boost_program_options_SOURCE_DIR}/src/config_file.cpp
    ${boost_program_options_SOURCE_DIR}/src/convert.cpp
    ${boost_program_options_SOURCE_DIR}/src/options_description.cpp
    ${boost_program_options_SOURCE_DIR}/src/parsers.cpp
    ${boost_program_options_SOURCE_DIR}/src/positional_options.cpp
    ${boost_program_options_SOURCE_DIR}/src/split.cpp
    ${boost_program_options_SOURCE_DIR}/src/utf8_codecvt_facet.cpp
    ${boost_program_options_SOURCE_DIR}/src/value_semantic.cpp
    ${boost_program_options_SOURCE_DIR}/src/variables_map.cpp
    ${boost_program_options_SOURCE_DIR}/src/winmain.cpp
)
target_include_directories(boost_program_options_lib PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${boost_program_options_SOURCE_DIR}/include
)

add_executable(linear_regression src/main.cpp)

target_include_directories(linear_regression PRIVATE include)
target_link_libraries(linear_regression PRIVATE rapidcsv boost_program_options_lib)

# `cmake --build . --target run` (or `make run`): build then execute the binary
add_custom_target(run
    COMMAND linear_regression ${ARGS}
    DEPENDS linear_regression
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running linear_regression"
    USES_TERMINAL
)

enable_testing()
add_subdirectory(tests)
