#ifndef WORLD_HPP
#define WORLD_HPP

#include "Entity.hpp"
#include "../factories/EntityFactory.hpp" // Include the EntityFactory interface
#include <vector>
#include <memory>
#include <algorithm>

namespace Logic {

    struct Rectangle {
        float x;
        float y;
        float width;
        float height;
    };

    class World {
    public:
        static constexpr int width = 20;
        static constexpr int height = 11;

        World(std::shared_ptr<EntityFactory> factory); // Constructor with factory
        ~World();

        void addEntity(EntityType type, int x, int y);
        void removeEntity(int x, int y);
        const std::vector<std::unique_ptr<Entity>>& getEntities() const;
        void update(float deltaTime);
        bool checkCollision(const Rectangle& rect1, const Rectangle& rect2) const;

        Pacman* getPacman();


    private:
        std::vector<std::unique_ptr<Entity>> entities;
        EntityType map[height][width];
        std::shared_ptr<EntityFactory> entityFactory; // Factory member

        // Helper methods
        Rectangle getEntityBounds(const Entity& entity) const;
        void updatePacmanPosition(Pacman &pacman, float deltaTime);
        void checkPacmanCollisions(Pacman &pacman);
        void updateGhostPosition(Ghost &ghost, float deltaTime);
        void checkGhostCollisions(Ghost &ghost);
    };

} // namespace Logic

#endif // WORLD_HPP
