// tests/utils_tests/RandomTest.cpp
#include "utils/Random.hpp"
#include <iostream>
#include <cassert>

using namespace utils;

int testRandom() {
    // Test 1: Check if the same instance is returned
    std::cout << "Running Test 1: Check if the same instance is returned...\n";
    Random& random1 = Random::getInstance();
    Random& random2 = Random::getInstance();
    assert(&random1 == &random2);
    std::cout << "Test 1 passed.\n";

    // Test 2: Check if the generated integer is within the specified range
    std::cout << "\nRunning Test 2: Check if the generated integer is within the specified range...\n";
    int lowerInt = 1, upperInt = 10;
    int randomInt = random1.getInt(lowerInt, upperInt);
    assert(randomInt >= lowerInt && randomInt <= upperInt);
    std::cout << "Generated integer: " << randomInt << "\n";
    std::cout << "Test 2 passed.\n";

    // Test 3: Check if the generated double is within the specified range
    std::cout << "\nRunning Test 3: Check if the generated double is within the specified range...\n";
    double lowerDouble = 0.0, upperDouble = 1.0;
    double randomDouble = random1.getDouble(lowerDouble, upperDouble);
    assert(randomDouble >= lowerDouble && randomDouble < upperDouble);
    std::cout << "Generated double: " << randomDouble << "\n";
    std::cout << "Test 3 passed.\n";

    return 0;
}