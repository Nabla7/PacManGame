// World.cpp

#include "../../../Logic/models/World.hpp"
#include <map>
#include <iostream>
#include <cmath>

namespace Logic {

World::World(std::shared_ptr<EntityFactory> factory) : entityFactory(std::move(factory)) {
    // Hardcoded map represented by digits
    int initialMap[height][width] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1},
            {1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1},
            {1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1},
            {1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1},
            {1, 0, 2, 0, 1, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1},
            {1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1},
            {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 3, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
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
    if (x >= 0 && x < width && y >= 0 && y < height) {
        std::unique_ptr<Entity> entity;
        switch (type) {
            case EntityType::Coin:
                entity = entityFactory->createCoin();
                break;
            case EntityType::Fruit:
                entity = entityFactory->createFruit();
                break;
            case EntityType::Ghost:
                entity = entityFactory->createGhost();
                break;
            case EntityType::Wall:
                entity = entityFactory->createWall();
                break;
            case EntityType::Pacman:
                entity = entityFactory->createPacman();
                break;
            default:
                return; // Do nothing for EntityType::Empty
        }

        entity->position.x = (static_cast<float>(x) / static_cast<float>(width - 1)) * 2.0f - 1.0f;
        entity->position.y = ((static_cast<float>(y) / static_cast<float>(height - 1)) * 2.0f - 1.0f);


        entities.push_back(std::move(entity));
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
    }
}

const std::vector<std::unique_ptr<Entity>>& World::getEntities() const {
    return entities;
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
    Rectangle bounds;

    // Convert to normalized coordinates
    float normWidth = 1.0f / width * 2.0f;
    float normHeight = 1.0f / height * 2.0f;

    bounds.x = entity.position.x;
    bounds.y = entity.position.y;

    bounds.width = normWidth;
    bounds.height = normHeight;

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
    // Get the current direction and speed of the Pacman
    Entity::Direction direction = pacman.direction_;
    Entity::Direction desiredDirection = pacman.desiredDirection_;
    float speed = pacman.speed;

    // Convert normalized position to grid coordinates
    int gridX = (pacman.position.x + 1.0f) / 2.0f * width;
    int gridY = (pacman.position.y + 1.0f) / 2.0f * height;

    // Check for walls in adjacent tiles
    bool wallAbove = isWallAbove(gridX, gridY);
    bool wallBelow = isWallBelow(gridX, gridY);
    bool wallLeft = isWallLeft(gridX, gridY);
    bool wallRight = isWallRight(gridX, gridY);

    // Check feasibility for the desired direction
    // Check if the desired direction is feasible and if Pacman is aligned with the grid
    bool canMoveInDesiredDirection = false;
    bool isAlignedWithGrid = isPacmanAlignedWithGrid(pacman);

    switch (pacman.desiredDirection_) {
        case Entity::Direction::Up:
            canMoveInDesiredDirection = !wallAbove && isAlignedWithGrid;
            break;
        case Entity::Direction::Down:
            canMoveInDesiredDirection = !wallBelow && isAlignedWithGrid;
            break;
        case Entity::Direction::Left:
            canMoveInDesiredDirection = !wallLeft && isAlignedWithGrid;
            break;
        case Entity::Direction::Right:
            canMoveInDesiredDirection = !wallRight && isAlignedWithGrid;
            break;
    }

    // Update direction if feasible
    if (canMoveInDesiredDirection) {
        pacman.direction_ = pacman.desiredDirection_;
    }

    // Calculate the new position based on the direction and speed
    auto newPosition = pacman.position;
    if (direction == Entity::Direction::Up) {
        newPosition.y -= speed * deltaTime;
    } else if (direction == Entity::Direction::Down) {
        newPosition.y += speed * deltaTime;
    } else if (direction == Entity::Direction::Left) {
        newPosition.x -= speed * deltaTime;
    } else if (direction == Entity::Direction::Right) {
        newPosition.x += speed * deltaTime;
    }


    // Create a rectangle for the new position
    Rectangle newBounds;
    newBounds.x = newPosition.x;
    newBounds.y = newPosition.y;
    newBounds.width = 1.0f / width * 2.0f;  // Scaling to normalized coordinates
    newBounds.height = 1.0f / height * 2.0f;


    // Check for a collision with a wall
    for (const auto& entity : entities) {
        if (entity->getType() == EntityType::Wall) {
            Rectangle wallBounds = getEntityBounds(*entity);
            if (checkCollision(newBounds, wallBounds)) {
                    // Collision detected, don't update Pacman's position
                    return;

            }
        }
    }


    // No collision detected, update Pacman's position
    pacman.position = newPosition;
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

Pacman* World::getPacman() {
    for (auto& entity : entities) {
        if (auto pacman = dynamic_cast<Pacman*>(entity.get())) {
            return pacman;
        }
    }
    return nullptr; // Return nullptr if no Pacman found
}

bool World::isWallAbove(int gridX, int gridY) {
    if (gridY - 1 >= 0) {
        return map[gridY - 1][gridX] == EntityType::Wall;
    }
    return false;
}

bool World::isWallBelow(int gridX, int gridY) {
    if (gridY + 1 < height) {
        return map[gridY + 1][gridX] == EntityType::Wall;
    }
    return false;
}

bool World::isWallLeft(int gridX, int gridY) {
    if (gridX - 1 >= 0) {
        return map[gridY][gridX - 1] == EntityType::Wall;
    }
    return false;
}

bool World::isWallRight(int gridX, int gridY) {
    if (gridX + 1 < width) {
        return map[gridY][gridX + 1] == EntityType::Wall;
    }
    return false;
}

bool World::isPacmanAlignedWithGrid(const Pacman& pacman) {
    // Determine if Pacman's position is close enough to the grid lines
    // This might involve checking if Pacman's position is near integer values
    // Adjust the threshold based on how strict you want the alignment check to be
    const float alignmentThreshold = 0.45f;
    float gridPosX = (pacman.position.x + 1.0f) / 2.0f * width;
    float gridPosY = (pacman.position.y + 1.0f) / 2.0f * height;

    bool alignedX = std::fabs(gridPosX - std::round(gridPosX)) < alignmentThreshold;
    bool alignedY = std::fabs(gridPosY - std::round(gridPosY)) < alignmentThreshold;

    return alignedX && alignedY;
}


} // namespace Logic