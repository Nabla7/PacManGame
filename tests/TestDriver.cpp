#include "logic_tests/utils_tests/StopwatchTest.cpp"
#include "logic_tests/utils_tests/RandomTest.cpp"
#include "logic_tests/models_tests/WorldTest.cpp"
#include "logic_tests/models_tests/EntityTest.cpp"
#include "representation_tests/GameTest.cpp"
#include "representation_tests/EntityPositionTest.cpp"
#include "representation_tests/SpriteTest.cpp"
#include "logic_tests/models_tests/WorldAddEntityTest.cpp"
#include "representation_tests/CameraProjectPositionTest.cpp"

int main() {
    std::cout << "\n========== Stopwatch Tests ==========\n";
    testStopwatch();
    std::cout << "\n========== Random Tests ==========\n";
    testRandom();
    std::cout << "\n========== World Tests ==========\n";
    testWorld();
    std::cout << "\n========== Entity Tests ==========\n";
    testEntities();
    std::cout << "\n========== Game Tests ==========\n";
    testGame();
    std::cout << "\n========== Entity Position Tests ==========\n";
    testEntityPositions();
    std::cout << "\n========== Sprite Tests ==========\n";
    testSprites();
    std::cout << "\n========== World Add Entity Tests ==========\n";
    testWorldAddEntity();
    std::cout << "\n========== Camera Project Position Tests ==========\n";
    testCameraProjectPosition();
    return 0;
}
