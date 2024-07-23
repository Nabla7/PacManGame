// World.cpp

#include "../../../Logic/models/World.hpp"
#include "../../../Logic/utils/Observer.hpp"
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

    // Add observer for keeping track of the score
    eventSubject.attach(&scoreObserver);
}

int World::getScore() const {
    return scoreObserver.getCurrentScore();
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

            // Set the position directly without normalization
            entity->position.x = static_cast<double>(x);
            entity->position.y = static_cast<double>(y);

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
        bounds.x = entity.position.x;
        bounds.y = entity.position.y;
        bounds.width = entity.getSize().first / static_cast<double>(width) * 2.0;
        bounds.height = entity.getSize().second / static_cast<double>(height) * 2.0;
        return bounds;
    }



void World::update(double deltaTime) {
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
        // Notify observers before removing the entity
        eventSubject.notify(entityToRemove->getType());
        removeEntity(entityToRemove);
    }
}


    void World::updatePacmanPosition(Pacman& pacman, double deltaTime) {
        Entity::Direction direction = pacman.direction_;
        Entity::Direction desiredDirection = pacman.desiredDirection_;
        double speed = pacman.speed;

        pacman.direction_ = pacman.desiredDirection_;

        auto newPosition = pacman.position;
        double moveDistance = speed * deltaTime * 5.0; // Increase movement speed
        if (direction == Entity::Direction::Up) {
            newPosition.y -= moveDistance;
        } else if (direction == Entity::Direction::Down) {
            newPosition.y += moveDistance;
        } else if (direction == Entity::Direction::Left) {
            newPosition.x -= moveDistance;
        } else if (direction == Entity::Direction::Right) {
            newPosition.x += moveDistance;
        }

        // Check for collisions and update position
        if (!checkCollisionWithWalls(newPosition)) {
            pacman.position = newPosition;
        }
    }

    bool World::checkCollisionWithWalls(const Entity::Position& position) {
        int gridX = static_cast<int>(std::round(position.x));
        int gridY = static_cast<int>(std::round(position.y));

        // Check if the new position is within bounds and not a wall
        if (gridX >= 0 && gridX < width && gridY >= 0 && gridY < height) {
            return map[gridY][gridX] == EntityType::Wall;
        }
        return true; // Treat out-of-bounds as a collision
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


void World::updateGhostPosition(Ghost& ghost, double deltaTime) {
    // Implement Ghost AI and movement logic
    // Handle collisions with walls and adjust position
}

void World::checkGhostCollisions(Ghost& ghost) {
    // Check for collisions with Pacman and handle appropriately
}

Pacman * World::getPacman() const {
    for (auto& entity : entities) {
        if (auto pacman = dynamic_cast<Pacman*>(entity.get())) {
            return pacman;
        }
    }
    return nullptr; // Return nullptr if no Pacman found
}

} // namespace Logic