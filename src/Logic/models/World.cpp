// World.cpp

#include "../../../Logic/models/World.hpp"
#include <map>

namespace Logic {

World::World() {
    // Hardcoded map represented by digits
    int initialMap[height][width] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
    {1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3, 1},
    {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
    {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
    {1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1},
    {1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 4, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 4, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

    // Mapping from digits to EntityType values
    std::map<int, EntityType> entityTypeMap = {
        {0, EntityType::Empty},
        {1, EntityType::Wall},
        {2, EntityType::Coin},
        {3, EntityType::Fruit},
        {4, EntityType::Ghost},
        {5, EntityType::Pacman}
    };

    // Copy the initial map to the world's map
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            EntityType type = entityTypeMap[initialMap[y][x]];
            map[y][x] = type;
            if (type != EntityType::Empty) {
                addEntity(type, x, y);
            }
        }
    }
}

World::~World() {
    // Unique pointers in entities vector will automatically deallocate memory
}

void World::addEntity(EntityType type, int x, int y) {
    // Ensure the coordinates are within the world bounds
    if (x >= 0 && x < width && y >= 0 && y < height) {
        // Depending on the EntityType, create the appropriate entity
        switch (type) {
            case EntityType::Coin:
                entities.push_back(std::make_unique<Coin>());
                break;
            case EntityType::Fruit:
                entities.push_back(std::make_unique<Fruit>());
                break;
            case EntityType::Ghost:
                entities.push_back(std::make_unique<Ghost>());
                break;
            case EntityType::Wall:
                entities.push_back(std::make_unique<Wall>());
                break;
            case EntityType::Pacman:
                entities.push_back(std::make_unique<Pacman>());
                break;
            default:
                break; // Do nothing for EntityType::Empty
        }
        // Set the entity's position
        entities.back()->position.x = x;
        entities.back()->position.y = y;
        // Update the map
        map[y][x] = type;
    }
}

void World::removeEntity(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        // Find and remove the entity from the vector
        auto it = std::remove_if(entities.begin(), entities.end(),
            [x, y](const std::unique_ptr<Entity>& entity) {
                return entity->position.x == x && entity->position.y == y;
            });
        entities.erase(it, entities.end());
        // Update the map
        map[y][x] = EntityType::Empty;
    }
}

bool World::checkCollision(const Rectangle& rect1, const Rectangle& rect2) const {
    // Check if rectangles intersect
    if (rect1.x < rect2.x + rect2.width &&
        rect1.x + rect1.width > rect2.x &&
        rect1.y < rect2.y + rect2.height &&
        rect1.height + rect1.y > rect2.y) {
        return true;
    }
    return false;
}

Rectangle World::getEntityBounds(const Entity& entity) const {
    // Placeholder for actual implementation
    // This should return the bounding rectangle of the entity
    Rectangle bounds;
    // Calculate bounds based on the entity's position and size
    // For example:
    bounds.x = entity.position.x;
    bounds.y = entity.position.y;
    bounds.width = 1.0f; // Placeholder value
    bounds.height = 1.0f; // Placeholder value
    return bounds;
}

void World::update(float deltaTime) {
    // Update the state of the world and its entities
    for (auto& entity : entities) {
        // Dynamically cast to check entity type and update accordingly
        if (auto pacman = dynamic_cast<Pacman*>(entity.get())) {
            // Update Pacman's position based on direction and speed
            updatePacmanPosition(*pacman, deltaTime);
            checkPacmanCollisions(*pacman);
        } else if (auto ghost = dynamic_cast<Ghost*>(entity.get())) {
            // Update Ghost's position and behavior
            updateGhostPosition(*ghost, deltaTime);
            checkGhostCollisions(*ghost);
        } else if (auto coin = dynamic_cast<Coin*>(entity.get())) {
            // For coins might want to check if they need to be collected
            //checkCoinCollection(*coin);
        }
        // similar logic for Fruit and Wall
    }
    // Additional game logic here (e.g., checking game state)
}

void World::updatePacmanPosition(Pacman& pacman, float deltaTime) {
    // Implement Pacman movement logic
    // e.g., pacman.position.x += pacman.speed * deltaTime * direction_x;
    // Handle collisions with walls and adjust position
}

void World::checkPacmanCollisions(Pacman& pacman) {
    // Check for collisions with other entities like coins, fruits, ghosts
    // Handle each collision appropriately (e.g., increase score, change game state)
}

void World::updateGhostPosition(Ghost& ghost, float deltaTime) {
    // Implement Ghost AI and movement logic
    // Handle collisions with walls and adjust position
}

void World::checkGhostCollisions(Ghost& ghost) {
    // Check for collisions with Pacman and handle appropriately
}

// Define similar functions for other entities if needed

} // namespace Logic