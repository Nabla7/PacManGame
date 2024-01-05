// tests/representation_tests/CameraProjectPositionTest.cpp
#include "Camera.hpp"
#include <iostream>
#include <cassert>

using namespace Representation;

int testCameraProjectPosition() {
    Camera camera(800, 600);

    // Test 1: Check if normalized position is converted to correct pixel coordinates
    std::cout << "Running Test 1: Check if normalized position is converted to correct pixel coordinates...\n";
    auto [pixelX, pixelY] = camera.projectPosition(0.0f, 0.0f); // Center of the screen
    assert(pixelX == 400.0f && pixelY == 300.0f);
    std::cout << "Test 1 passed.\n";

    // More tests...

    return 0;
}