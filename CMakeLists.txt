cmake_minimum_required(VERSION 3.2)
project(PacManGame)

set(CMAKE_CXX_STANDARD 17)

# Define the path to SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/libs/SFML-2.5.1-macos-clang/lib/cmake/SFML")

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Add the Logic and Representation directories to the include paths
include_directories(${CMAKE_SOURCE_DIR}/Logic)
include_directories(${CMAKE_SOURCE_DIR}/Representation)

# Collect all source files recursively
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the main executable
add_executable(PacManGame main.cpp ${SOURCES})

# Link SFML libraries
target_link_libraries(PacManGame sfml-graphics sfml-audio)

# Copy the assets to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# If tests are enabled, add them
option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    enable_testing()
    set(TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/TestDriver.cpp" ${SOURCES})
    add_executable(PacManTests ${TEST_SOURCES})
    target_link_libraries(PacManTests sfml-graphics sfml-audio)
endif()
