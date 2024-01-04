// EntityTest.cpp
#include "models/Entity.hpp"
#include <iostream>
#include <cassert>

using namespace Logic;

int testEntities() {
    // Test Coin
    Coin coin;
    coin.setValue(10);
    std::cout << "Running Test 1: Coin Value...\n";
    assert(coin.getValue() == 10);
    std::cout << "Coin value set and retrieved successfully. Test 1 passed.\n";

    // Test Pacman
    Pacman pacman;
    pacman.setLives(3);
    std::cout << "Running Test 2: Pacman Lives...\n";
    assert(pacman.getLives() == 3);
    std::cout << "Pacman lives set and retrieved successfully. Test 2 passed.\n";

    // More tests...

    return 0;
}