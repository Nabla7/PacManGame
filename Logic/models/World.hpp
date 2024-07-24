#ifndef WORLD_HPP
#define WORLD_HPP

#include "Entity.hpp"
#include "../factories/EntityFactory.hpp" // Include the EntityFactory interface
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

        void addEntity(EntityType type, int x, int y);
        void removeEntity(Entity *entityToRemove);
        const std::vector<std::unique_ptr<Entity>>& getEntities() const;
        void update(double deltaTime);
        bool checkCollision(const Rectangle& rect1, const Rectangle& rect2) const;

        Pacman * getPacman() const;

        void updateGhostPosition(Ghost& ghost, double deltaTime, int level);
        Entity::Direction chooseGhostDirection(const Ghost& ghost, const Pacman& pacman);
        int getManhattanDistance(const Entity::Position& pos1, const Entity::Position& pos2) const;
        std::vector<Entity::Direction> getViableDirections(const Ghost& ghost) const;

        int getScore() const;

    private:
        std::vector<std::unique_ptr<Entity>> entities;
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
    };

} // namespace Logic

#endif // WORLD_HPP
