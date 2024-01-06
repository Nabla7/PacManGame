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
            {1, 3, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1},
            {1, 2, 1, 1, 2, 1, 4, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1},
            {1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
            {1, 2, 1, 2, 1, 1, 2, 1, 1, 0, 0, 1, 1, 2, 1, 1, 2, 1, 2, 1},
            {1, 0, 5, 2, 2, 2, 2, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 1},
            {1, 0, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1},
            {1, 0, 1, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
            {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1},
            {1, 0, 0, 0, 4, 1, 0, 0, 0, 0, 2, 2, 2, 2, 1, 2, 2, 2, 3, 1},
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

void World::removeEntity(Entity* entityToRemove) {
    // Find and remove the entity from the vector
    auto it = std::remove_if(entities.begin(), entities.end(),
        [entityToRemove](const std::unique_ptr<Entity>& entity) {
            return entity.get() == entityToRemove;
        });
    entities.erase(it, entities.end());
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

    bounds.width = normWidth*0.95;
    bounds.height = normHeight*0.95;

    return bounds;
}



void World::update(float deltaTime) {
    std::vector<Entity*> entitiesToRemove;

    // Update logic for all entities
    for (auto& entity : entities) {
        switch (entity->getType()) {
            case EntityType::Pacman: {
                auto pacman = static_cast<Pacman*>(entity.get());
                updatePacmanPosition(*pacman, deltaTime);
                checkPacmanCollisions(*pacman, entitiesToRemove);
                break;
            }
            case EntityType::Ghost: {
                auto ghost = static_cast<Ghost*>(entity.get());
                updateGhostPosition(*ghost, deltaTime);
                checkGhostCollisions(*ghost);
                break;
            }
                // Handle other entity types if necessary
            default:
                break;
        }
    }

    // Remove entities marked for deletion
    for (auto entityToRemove : entitiesToRemove) {
        removeEntity(entityToRemove);
    }
}


void World::updatePacmanPosition(Pacman& pacman, float deltaTime) {
    // Get the current direction and speed of the Pacman
    Entity::Direction direction = pacman.direction_;
    Entity::Direction desiredDirection = pacman.desiredDirection_;
    float speed = pacman.speed;
    
    pacman.direction_ = pacman.desiredDirection_;

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
    newBounds.width = (1.0f / width * 2.0f)*0.95;  // Scaling to normalized coordinates
    newBounds.height = (1.0f / height * 2.0f)*0.95;


    // Check for a collision with a wall
    for (const auto& entity : entities) {
        if (entity->getType() == EntityType::Wall) {
            Rectangle wallBounds = getEntityBounds(*entity);
            if (checkCollision(newBounds, wallBounds)) {
                std::cout << "Collision detected with wall. Pacman's position will not be updated." << std::endl;
                // Collision detected, don't update Pacman's position
                return;

            }
        }
    }


    // No collision detected, update Pacman's position
    pacman.position = newPosition;
}

void World::checkPacmanCollisions(Pacman& pacman, std::vector<Entity*>& entitiesToRemove) {
    Rectangle pacmanBounds = getEntityBounds(pacman);

    for (auto& entity : entities) {
        if (entity->getType() == EntityType::Coin) {
            Rectangle coinBounds = getEntityBounds(*entity);
            if (checkCollision(pacmanBounds, coinBounds)) {
                // Collision with a coin detected
                entitiesToRemove.push_back(entity.get());
            }
        }else if (entity->getType() == EntityType::Fruit){
            Rectangle fruitBounds = getEntityBounds(*entity);
            if (checkCollision(pacmanBounds, fruitBounds)) {
                // Collision with a fruit detected
                entitiesToRemove.push_back(entity.get());
            }
        }
    }
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








} // namespace Logic