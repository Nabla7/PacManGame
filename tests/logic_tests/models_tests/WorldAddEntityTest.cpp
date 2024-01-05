// tests/logic_tests/models_tests/WorldAddEntityTest.cpp
#include "models/World.hpp"
#include "factories/ConcreteEntityFactory.hpp"
#include <iostream>
#include <cassert>

using namespace Logic;

int testWorldAddEntity() {
    std::shared_ptr<EntityFactory> factory = std::make_shared<PacmanGameEntityFactory>();
    World world(factory);

    // Test 1: Check if entity is added at correct normalized position
    std::cout << "Running Test 1: Check if entity is added at correct normalized position...\n";
    world.addEntity(EntityType::Coin, 14, 15); // Add a coin at the center of the map
    const auto& entities = world.getEntities();
    float actualX = entities.back()->position.x;
    float actualY = entities.back()->position.y;
    std::cout << "Expected: (0.0, 0.0), Actual: (" << actualX << ", " << actualY << ")\n";
    assert(actualX == 0.0f && actualY == 0.0f);
    std::cout << "Test 1 passed.\n";

    // More tests...

    return 0;
}