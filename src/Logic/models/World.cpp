// World.cpp

#include "../../../Logic/models/World.hpp"
#include "../../../Logic/utils/Observer.hpp"
#include "utils/Random.hpp"
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
        return (rect1.x < rect2.x + rect2.width &&
                rect1.x + rect1.width > rect2.x &&
                rect1.y < rect2.y + rect2.height &&
                rect1.y + rect1.height > rect2.y);
    }

    Rectangle World::getEntityBounds(const Entity& entity) const {
        Rectangle bounds;
        bounds.x = entity.position.x;
        bounds.y = entity.position.y;
        bounds.width = entity.getSize().first;
        bounds.height = entity.getSize().second;
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
        Entity::Direction currentDirection = pacman.direction_;
        Entity::Direction desiredDirection = pacman.desiredDirection_;
        double speed = pacman.speed;

        auto tryMove = [&](Entity::Direction dir) -> bool {
            auto newPosition = pacman.position;
            double moveDistance = speed * deltaTime * 5.0;

            switch (dir) {
                case Entity::Direction::Up:    newPosition.y -= moveDistance; break;
                case Entity::Direction::Down:  newPosition.y += moveDistance; break;
                case Entity::Direction::Left:  newPosition.x -= moveDistance; break;
                case Entity::Direction::Right: newPosition.x += moveDistance; break;
            }

            Rectangle pacmanRect = {newPosition.x, newPosition.y, pacman.getSize().first, pacman.getSize().second};

            for (const auto& entity : entities) {
                if (entity->getType() == EntityType::Wall) {
                    Rectangle wallRect = getEntityBounds(*entity);
                    if (checkCollision(pacmanRect, wallRect)) {
                        return false;
                    }
                }
            }

            pacman.position = newPosition;
            return true;
        };

        // First, try to move in the desired direction
        if (tryMove(desiredDirection)) {
            pacman.direction_ = desiredDirection;
        }
            // If that fails and it's different from the current direction, try to move in the current direction
        else if (desiredDirection != currentDirection && tryMove(currentDirection)) {
            // Keep the current direction
        }
        // If both fail, Pacman stops (which is handled by not updating the position)
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
        if (ghost.state == Ghost::State::Waiting && elapsedTime >= ghost.spawnTimer) {
            ghost.state = Ghost::State::Chasing;
        }

        if (ghost.state == Ghost::State::Chasing) {
            auto newPosition = ghost.position;
            double moveDistance = ghost.speed * deltaTime * 5.0;

            switch (ghost.lockedDirection) {
                case Entity::Direction::Up:    newPosition.y -= moveDistance; break;
                case Entity::Direction::Down:  newPosition.y += moveDistance; break;
                case Entity::Direction::Left:  newPosition.x -= moveDistance; break;
                case Entity::Direction::Right: newPosition.x += moveDistance; break;
            }

            Rectangle ghostRect = {newPosition.x, newPosition.y, ghost.getSize().first, ghost.getSize().second};

            bool collision = false;
            for (const auto& entity : entities) {
                if (entity->getType() == EntityType::Wall) {
                    Rectangle wallRect = getEntityBounds(*entity);
                    if (checkCollision(ghostRect, wallRect)) {
                        collision = true;
                        break;
                    }
                }
            }

            if (!collision) {
                ghost.position = newPosition;
            } else {
                Pacman* pacman = getPacman();
                if (pacman) {
                    ghost.lockedDirection = chooseGhostDirection(ghost, *pacman);
                }
            }
        }
    }

    Entity::Direction World::chooseGhostDirection(const Ghost& ghost, const Pacman& pacman) {
        std::vector<Entity::Direction> viableDirections = getViableDirections(ghost);

        if (viableDirections.empty()) {
            return ghost.lockedDirection;
        }

        if (utils::Random::getInstance().getDouble(0, 1) < 0.5) {
            return viableDirections[utils::Random::getInstance().getInt(0, viableDirections.size() - 1)];
        }

        int minDistance = std::numeric_limits<int>::max();
        std::vector<Entity::Direction> bestDirections;

        for (const auto& dir : viableDirections) {
            Entity::Position newPos = ghost.position;
            switch (dir) {
                case Entity::Direction::Up:    newPos.y -= 1; break;
                case Entity::Direction::Down:  newPos.y += 1; break;
                case Entity::Direction::Left:  newPos.x -= 1; break;
                case Entity::Direction::Right: newPos.x += 1; break;
            }

            int distance = getManhattanDistance(newPos, pacman.position);
            if (distance < minDistance) {
                minDistance = distance;
                bestDirections.clear();
                bestDirections.push_back(dir);
            } else if (distance == minDistance) {
                bestDirections.push_back(dir);
            }
        }

        return bestDirections[utils::Random::getInstance().getInt(0, bestDirections.size() - 1)];
    }

    int World::getManhattanDistance(const Entity::Position& pos1, const Entity::Position& pos2) const {
        return std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y);
    }

    std::vector<Entity::Direction> World::getViableDirections(const Ghost& ghost) const {
        std::vector<Entity::Direction> viableDirections;
        const double step = 0.1; // Small step to check for collisions

        for (const auto& dir : {Entity::Direction::Up, Entity::Direction::Down, Entity::Direction::Left, Entity::Direction::Right}) {
            Entity::Position newPos = ghost.position;
            switch (dir) {
                case Entity::Direction::Up:    newPos.y -= step; break;
                case Entity::Direction::Down:  newPos.y += step; break;
                case Entity::Direction::Left:  newPos.x -= step; break;
                case Entity::Direction::Right: newPos.x += step; break;
            }

            Rectangle ghostRect = {newPos.x, newPos.y, ghost.getSize().first, ghost.getSize().second};
            bool collision = false;

            for (const auto& entity : entities) {
                if (entity->getType() == EntityType::Wall) {
                    Rectangle wallRect = getEntityBounds(*entity);
                    if (checkCollision(ghostRect, wallRect)) {
                        collision = true;
                        break;
                    }
                }
            }

            if (!collision) {
                viableDirections.push_back(dir);
            }
        }

        return viableDirections;
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