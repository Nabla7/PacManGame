// World.cpp

#include "../../../Logic/models/World.hpp"
#include "../../../Logic/utils/Observer.hpp"
#include "utils/Random.hpp"
#include <map>
#include <iostream>
#include <cmath>
#include <queue>
#include <cmath>
#include <algorithm>

namespace Logic {

    World::World(std::shared_ptr<EntityFactory> factory) : entityFactory(std::move(factory)) {
        // Hardcoded map represented by digits
        int initialMap[height][width] = {
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 3, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1},
                {1, 2, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1},
                {1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
                {1, 2, 1, 2, 1, 1, 2, 1, 1, 0, 0, 1, 1, 2, 1, 1, 2, 1, 2, 1},
                {1, 0, 5, 2, 2, 2, 2, 1, 4, 4, 4, 4, 1, 2, 2, 2, 2, 2, 2, 1},
                {1, 0, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1},
                {1, 0, 1, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1},
                {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1},
                {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 2, 2, 2, 1, 2, 2, 2, 3, 1},
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

        totalCoins = 0;
        totalFruits = 0;
        eatenCoins = 0;
        eatenFruits = 0;

        // Copy the initial map to the world's map
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                EntityType type = entityTypeMap[initialMap[y][x]];
                map[y][x] = type;
                if (type != EntityType::Empty) {
                    auto entityPtr = addEntity(type, x, y);
                    if (entityPtr) {
                        if (type == EntityType::Coin) {
                            totalCoins++;
                        } else if (type == EntityType::Fruit) {
                            totalFruits++;
                        } else if (type == EntityType::Pacman) {
                            if (auto pacman = dynamic_cast<Pacman*>(entityPtr.get())) {
                                pacman->setLives(3); // Set initial lives for Pacman
                            }
                        }
                    }
                }
            }
        }

        // Add observer for keeping track of the score
        eventSubject.attach(&scoreObserver);
    }

    World::~World() {
        // Unique pointers in entities vector will automatically deallocate memory
    }

    int World::getScore() const {
        return scoreObserver.getCurrentScore();
    }

    std::shared_ptr<Entity> World::addEntity(EntityType type, int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            std::shared_ptr<Entity> entity;
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
                    return nullptr; // Do nothing for EntityType::Empty
            }

            // Set the position
            entity->position.x = static_cast<double>(x);
            entity->position.y = static_cast<double>(y);

            entities.push_back(entity);
            map[y][x] = type;
            return entity;
        }
        return nullptr;
    }

    void World::removeEntity(Entity* entityToRemove) {
        auto it = std::remove_if(entities.begin(), entities.end(),
                                 [entityToRemove](const std::shared_ptr<Entity>& entity) {
                                     return entity.get() == entityToRemove;
                                 });
        entities.erase(it, entities.end());
    }

    const std::vector<std::shared_ptr<Entity>>& World::getEntities() const {
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
                    ghost->elapsedTime += deltaTime;
                    if (ghost->state == Ghost::State::Waiting && ghost->elapsedTime >= ghost->spawnDelay) {
                        ghost->state = Ghost::State::Chasing;
                    }
                    if (ghost->state == Ghost::State::Chasing) {
                        if (ghost->useSmartMovement) {
                            updateGhostPositionSmart(*ghost, deltaTime, currentLevel);
                        } else {
                            updateGhostPositionSimple(*ghost, deltaTime);
                        }
                    }
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


        if (allCoinsAndFruitsEaten()) {
            incrementLevel();
            resetEntities();
            respawnCoinsAndFruits();
        }

        eventSubject.notify(EntityType::Empty); // Using Empty as a general update signal
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
            if (entity->getType() == EntityType::Coin || entity->getType() == EntityType::Fruit) {
                Rectangle itemBounds = getEntityBounds(*entity);
                if (checkCollision(pacmanBounds, itemBounds)) {
                    entitiesToRemove.push_back(entity.get());

                    if (entity->getType() == EntityType::Coin) {
                        eatenCoins++;
                        std::cout << "Coin eaten. Total eaten: " << eatenCoins << "/" << totalCoins << std::endl;
                    } else if (entity->getType() == EntityType::Fruit) {
                        eatenFruits++;
                        std::cout << "Fruit eaten. Total eaten: " << eatenFruits << "/" << totalFruits << std::endl;
                    }

                    // Notify observers (for scoring)
                    eventSubject.notify(entity->getType());
                }
            }
        }
    }

    void World::updateGhostPositionSimple(Ghost& ghost, double deltaTime) {
        if (ghost.state == Ghost::State::Waiting && elapsedTime >= ghost.spawnDelay) {
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
        const double step = 1; // Small step to check for collisions

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

    Pacman* World::getPacman() const {
        for (auto& entity : entities) {
            if (auto pacman = dynamic_cast<Pacman*>(entity.get())) {
                return pacman;
            }
        }
        return nullptr; // Return nullptr if no Pacman found
    }

    std::vector<Entity::Position> World::findPath(const Ghost& ghost, const Pacman& pacman) {
        std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
        std::vector<std::vector<Node>> nodes(height, std::vector<Node>(width, Node(0, 0)));

        auto compare = [](const Node* a, const Node* b) { return a->f > b->f; };
        std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> openList(compare);

        int startX = static_cast<int>(std::round(ghost.position.x));
        int startY = static_cast<int>(std::round(ghost.position.y));
        int goalX = static_cast<int>(std::round(pacman.position.x));
        int goalY = static_cast<int>(std::round(pacman.position.y));

        nodes[startY][startX] = Node(startX, startY);
        openList.push(&nodes[startY][startX]);

        while (!openList.empty()) {
            Node* current = openList.top();
            openList.pop();

            if (current->x == goalX && current->y == goalY) {
                return reconstructPath(current);
            }

            visited[current->y][current->x] = true;

            for (const auto& dir : {Entity::Direction::Up, Entity::Direction::Down, Entity::Direction::Left, Entity::Direction::Right}) {
                int newX = current->x;
                int newY = current->y;

                switch (dir) {
                    case Entity::Direction::Up: newY--; break;
                    case Entity::Direction::Down: newY++; break;
                    case Entity::Direction::Left: newX--; break;
                    case Entity::Direction::Right: newX++; break;
                }

                if (newX < 0 || newX >= width || newY < 0 || newY >= height || visited[newY][newX] || map[newY][newX] == EntityType::Wall) {
                    continue;
                }

                double newG = current->g + 1.0;
                double newH = std::abs(newX - goalX) + std::abs(newY - goalY);
                double newF = newG + newH;

                Node* neighbor = &nodes[newY][newX];
                if (newG < neighbor->g || neighbor->g == 0) {
                    neighbor->x = newX;
                    neighbor->y = newY;
                    neighbor->g = newG;
                    neighbor->h = newH;
                    neighbor->f = newF;
                    neighbor->parent = current;
                    openList.push(neighbor);
                }
            }
        }

        return {}; // No path found
    }

    std::vector<Entity::Position> World::reconstructPath(Node* goal) {
        std::vector<Entity::Position> path;
        Node* current = goal;
        while (current != nullptr) {
            path.push_back({static_cast<double>(current->x), static_cast<double>(current->y)});
            current = current->parent;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    void World::updateGhostPositionSmart(Ghost& ghost, double deltaTime, int level) {
        if (ghost.state == Ghost::State::Waiting && elapsedTime >= ghost.spawnDelay) {
            ghost.state = Ghost::State::Chasing;
        }

        if (ghost.state == Ghost::State::Chasing) {
            Pacman* pacman = getPacman();
            if (pacman) {
                // Calculate difficulty factor using a sigmoid function
                double difficulty = 1.0 / (1.0 + std::exp(-0.1 * (level - 50)));

                std::vector<Entity::Position> path = findPath(ghost, *pacman);
                if (!path.empty() && path.size() > 1) {
                    Entity::Position nextPos = path[1]; // Next position after current

                    // Calculate direction vector
                    double dx = nextPos.x - ghost.position.x;
                    double dy = nextPos.y - ghost.position.y;
                    double length = std::sqrt(dx * dx + dy * dy);

                    if (length > 0) {
                        // Normalize direction vector
                        dx /= length;
                        dy /= length;

                        // Adjust direction based on difficulty
                        if (utils::Random::getInstance().getDouble(0, 1) > difficulty) {
                            // Randomly adjust direction for lower difficulties
                            dx += utils::Random::getInstance().getDouble(-0.5, 0.5) * (1 - difficulty);
                            dy += utils::Random::getInstance().getDouble(-0.5, 0.5) * (1 - difficulty);
                            length = std::sqrt(dx * dx + dy * dy);
                            dx /= length;
                            dy /= length;
                        }

                        // Move ghost
                        double moveDistance = ghost.speed * deltaTime * 5.0 * (0.5 + 0.5 * difficulty);
                        ghost.position.x += dx * moveDistance;
                        ghost.position.y += dy * moveDistance;

                        // Update locked direction based on primary movement axis
                        if (std::abs(dx) > std::abs(dy)) {
                            ghost.lockedDirection = (dx > 0) ? Entity::Direction::Right : Entity::Direction::Left;
                        } else {
                            ghost.lockedDirection = (dy > 0) ? Entity::Direction::Down : Entity::Direction::Up;
                        }
                    }
                }
            }
        }
    }

    void World::incrementLevel() {
        currentLevel++;
        // Increase difficulty here
        increaseGhostSpeed();
        decreaseFearModeDuration();
    }

    void World::resetLevel() {
        currentLevel = 1;
        // Reset difficulty here
        resetGhostSpeed();
        resetFearModeDuration();
    }

    bool World::allCoinsAndFruitsEaten() const {
        return eatenCoins == totalCoins && eatenFruits == totalFruits;
    }

    void World::increaseGhostSpeed() {
        ghostSpeedMultiplier *= 1.1; // Increase speed by 10% each level
    }

    void World::decreaseFearModeDuration() {
        fearModeDuration *= 0.9; // Decrease duration by 10% each level
    }

    void World::resetGhostSpeed() {
        ghostSpeedMultiplier = 1.0;
    }

    void World::resetFearModeDuration() {
        fearModeDuration = 10.0;
    }

    void World::prepareNextLevel() {
        resetEntities();
        respawnCoinsAndFruits();
    }

    void World::resetEntities() {
        // Implement the logic to reset entities to their starting positions
        for (auto& entity : entities) {
            if (auto ghost = dynamic_cast<Ghost*>(entity.get())) {
                // Reset ghost to starting position
                removeEntity(entity.get());

                auto ghost1 = std::dynamic_pointer_cast<Ghost>(addEntity(EntityType::Ghost, 9, 5));
                if (ghost1) ghost1->spawnDelay = 0.0;  // First ghost starts immediately

                auto ghost2 = std::dynamic_pointer_cast<Ghost>(addEntity(EntityType::Ghost, 10, 5));
                if (ghost2) ghost2->spawnDelay = 0.0;  // Second ghost also starts immediately

                auto ghost3 = std::dynamic_pointer_cast<Ghost>(addEntity(EntityType::Ghost, 11, 5));
                if (ghost3) ghost3->spawnDelay = 5.0;  // Third ghost starts after 5 seconds

                auto ghost4 = std::dynamic_pointer_cast<Ghost>(addEntity(EntityType::Ghost, 12, 5));
                if (ghost4) ghost4->spawnDelay = 10.0;  // Fourth ghost starts after 10 seconds
            }
            else if (auto pacman = dynamic_cast<Pacman*>(entity.get())) {
                // Reset Pacman to starting position
                pacman->position = {6, 1};
            }
        }
    }

    void World::respawnCoinsAndFruits() {
        // Implement the logic to respawn coins and fruits
        // For example:
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (map[y][x] == EntityType::Coin) {
                    addEntity(EntityType::Coin, x, y);
                }
                else if (map[y][x] == EntityType::Fruit) {
                    addEntity(EntityType::Fruit, x, y);
                }
            }
        }
        eatenCoins = 0;
        eatenFruits = 0;
    }

} // namespace Logic