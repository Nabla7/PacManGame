
#include "logic_tests/utils_tests/StopwatchTest.cpp"
#include "logic_tests/utils_tests/RandomTest.cpp"
#include "logic_tests/models_tests/WorldTest.cpp"
#include "logic_tests/models_tests/EntityTest.cpp"
#include "representation_tests/GameTest.cpp"

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
    return 0;
}

