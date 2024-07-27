#ifndef WORLD_HPP
#define WORLD_HPP

#include "Entity.hpp"
#include "../factories/EntityFactory.hpp"
#include "../utils/Observer.hpp"
#include <vector>
#include <memory>
#include <algorithm>

namespace Logic {

    struct Rectangle {
        double x;
        double y;
        double width;
        double height;
    };

    class World {
    public:
        static constexpr int width = 20;
        static constexpr int height = 11;

        World(std::shared_ptr<EntityFactory> factory); // Constructor with factory
        ~World();

        void update(double deltaTime);

        std::shared_ptr<Entity> addEntity(EntityType type, int x, int y);
        void removeEntity(Entity *entityToRemove);
        const std::vector<std::shared_ptr<Entity>>& getEntities() const;
        bool checkCollision(const Rectangle& rect1, const Rectangle& rect2) const;


        Pacman* getPacman() const;


        void updateGhostPositionSimple(Ghost& ghost, double deltaTime);
        void updateGhostPositionSmart(Ghost& ghost, double deltaTime, int level);
        Entity::Direction chooseGhostDirection(const Ghost& ghost, const Pacman& pacman);
        int getManhattanDistance(const Entity::Position& pos1, const Entity::Position& pos2) const;
        std::vector<Entity::Direction> getViableDirections(const Ghost& ghost) const;


        int getScore() const;
        void updateScore(double deltaTime);


        int getCurrentLevel() const { return currentLevel; }
        void incrementLevel();
        void resetLevel();
        void prepareNextLevel();
        bool allCoinsAndFruitsEaten() const;

        int currentLevel = 1;
        int totalCoins = 0;
        int totalFruits = 0;
        int eatenCoins = 0;
        int eatenFruits = 0;

        void attachObserver(IObserver* observer) {
            eventSubject.attach(observer);
        }

    private:
        std::vector<std::shared_ptr<Entity>> entities;
        std::vector<Ghost*> ghosts;
        double elapsedTime = 0.0;
        EntityType map[height][width];
        std::shared_ptr<EntityFactory> entityFactory; // Factory member
        Subject eventSubject;    // Subject for observer pattern
        Score scoreObserver;     // Observer for scoring

        // Helper methods
        Rectangle getEntityBounds(const Entity& entity) const;
        void updatePacmanPosition(Pacman &pacman, double deltaTime);
        void checkPacmanCollisions(Pacman& pacman, std::vector<Entity*>& entitiesToRemove);

        class Node {
        public:
            int x, y;
            double g, h, f;
            Node* parent;

            Node(int x, int y) : x(x), y(y), g(0), h(0), f(0), parent(nullptr) {}
        };

        std::vector<Entity::Position> reconstructPath(Node *goal);
        std::vector<Entity::Position> findPath(const Ghost &ghost, const Pacman &pacman);



        void resetEntities();

        void respawnCoinsAndFruits();

        double ghostSpeedMultiplier = 1.0;
        double fearModeDuration = 10.0; // in seconds

        void increaseGhostSpeed();
        void decreaseFearModeDuration();
        void resetGhostSpeed();
        void resetFearModeDuration();

    };

} // namespace Logic

#endif // WORLD_HPP
