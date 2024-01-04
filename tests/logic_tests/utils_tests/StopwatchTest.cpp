#include "utils/Stopwatch.hpp"
#include <thread>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace utils;

int testStopwatch() {
    Stopwatch& stopwatch = Stopwatch::getInstance();

    // Test 1: Elapsed time accuracy
    std::cout << "Running Test 1: Elapsed time accuracy..." << std::endl;
    stopwatch.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep for 100 milliseconds
    stopwatch.stop();

    double elapsed = stopwatch.getElapsedTime();
    std::cout << "Expected time: 0.1 seconds, Actual elapsed time: " << std::setprecision(3) << elapsed << " seconds" << std::endl;
    if (std::abs(elapsed - 0.1) < 0.01) { // Allowing a margin of error
        std::cout << "Test 1 passed." << std::endl;
    } else {
        std::cout << "Test 1 failed." << std::endl;
        return 1; // Failure
    }

    // Test 2: Frame rate capping
    std::cout << "\nRunning Test 2: Frame rate capping..." << std::endl;
    stopwatch.start();
    stopwatch.capFrameRate();
    stopwatch.stop();

    double frameTime = stopwatch.getElapsedTime();
    std::cout << "Minimum frame time: " << Stopwatch::getMaxFrameTime() << " seconds, Actual frame time: " << std::setprecision(3) << frameTime << " seconds" << std::endl;
    if (frameTime >= utils::Stopwatch::getMaxFrameTime()) {
        std::cout << "Test 2 passed." << std::endl;
    } else {
        std::cout << "Test 2 failed." << std::endl;
        return 1; // Failure
    }

    // Test 3: Stopwatch reset after stop
    std::cout << "\nRunning Test 3: Stopwatch reset after stop..." << std::endl;
    stopwatch.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    stopwatch.stop();

    stopwatch.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    stopwatch.stop();

    elapsed = stopwatch.getElapsedTime();
    std::cout << "Expected time after reset: 0.05 seconds, Actual elapsed time: " << std::setprecision(3) << elapsed << " seconds" << std::endl;
    if (std::abs(elapsed - 0.05) < 0.01) { // Checking if stopwatch resets after stop
        std::cout << "Test 3 passed." << std::endl;
    } else {
        std::cout << "Test 3 failed." << std::endl;
        return 1; // Failure
    }

    return 0; // Success
}
