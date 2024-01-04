
#include "models/World.hpp"
#include "factories/ConcreteEntityFactory.hpp"
#include <iostream>
#include <cassert>

using namespace Logic;

int testWorld() {
    std::shared_ptr<EntityFactory> factory = std::make_shared<PacmanGameEntityFactory>();
    World world(factory);

    // Test cases here...
    // Example:
    std::cout << "Running Test 1: World Initialization...\n";
    assert(true);
    std::cout << "Test 1 passed.\n";

    // More tests...

    return 0;
}

