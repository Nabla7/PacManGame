cmake_minimum_required(VERSION 3.16)
# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(PacManGame LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Set the path to your local SFML installation
set(SFML_DIR "${CMAKE_SOURCE_DIR}/libs/SFML-2.5.1-macos-clang/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)

# Add the Logic and Representation directories to the include paths
include_directories(${CMAKE_SOURCE_DIR}/Logic)
include_directories(${CMAKE_SOURCE_DIR}/Representation)

# Create the Logic library
file(GLOB_RECURSE LOGIC_SOURCES "src/Logic/*.cpp")
add_library(LogicLib STATIC ${LOGIC_SOURCES})
target_include_directories(LogicLib PUBLIC ${CMAKE_SOURCE_DIR}/Logic)

# Collect Representation source files
file(GLOB_RECURSE REPRESENTATION_SOURCES "src/Representation/*.cpp")

# Define the main executable
add_executable(PacManGame main.cpp ${REPRESENTATION_SOURCES})
target_link_libraries(PacManGame PRIVATE LogicLib sfml-graphics sfml-audio)
target_compile_features(PacManGame PRIVATE cxx_std_17)

# Copy the assets to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/bin)

# If tests are enabled, add them
option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    enable_testing()
    set(TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/TestDriver.cpp" ${REPRESENTATION_SOURCES})
    add_executable(PacManGameTests ${TEST_SOURCES})
    target_link_libraries(PacManGameTests PRIVATE LogicLib sfml-graphics sfml-audio)
endif()

# Install target
install(TARGETS PacManGame
        RUNTIME DESTINATION bin
        )

# Print SFML directories for debugging
message(STATUS "SFML_DIR: ${SFML_DIR}")