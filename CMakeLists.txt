cmake_minimum_required(VERSION 3.16)
# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(PacManGame LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# FetchContent for SFML
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Add the Logic and Representation directories to the include paths
include_directories(${CMAKE_SOURCE_DIR}/Logic)
include_directories(${CMAKE_SOURCE_DIR}/Representation)

# Collect all source files recursively
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the main executable
add_executable(PacManGame main.cpp ${SOURCES})
target_link_libraries(PacManGame PRIVATE sfml-graphics sfml-audio)
target_compile_features(PacManGame PRIVATE cxx_std_17)

# Copy the assets to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/bin)


# If tests are enabled, add them
option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    enable_testing()
    set(TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/TestDriver.cpp" ${SOURCES})
    add_executable(PacManGameTests ${TEST_SOURCES})
    target_link_libraries(PacManGameTests sfml-graphics sfml-audio)
endif()

# Windows specific copy command
if(WIN32)
    add_custom_command(
            TARGET PacManGame
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:PacManGame>
            VERBATIM)
endif()

install(TARGETS PacManGame)


