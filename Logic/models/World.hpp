// World.hpp

#ifndef WORLD_HPP
#define WORLD_HPP

#include "Entity.hpp" // Include the Entity class and its subclasses
#include <vector>
#include <memory>
#include <algorithm>

namespace Logic {

enum class EntityType {
    Empty = 0,
    Wall,
    Coin,
    Fruit,
    Ghost,
    Pacman
};

struct Rectangle {
    float x; // X coordinate of the top-left corner
    float y; // Y coordinate of the top-left corner
    float width; // Width of the rectangle
    float height; // Height of the rectangle
};

class World {
public:
    static constexpr int width = 29; // Width of the world in entity units
    static constexpr int height = 31; // Height of the world in entity units

    World();
    ~World();

    void addEntity(EntityType type, int x, int y);
    void removeEntity(int x, int y);
    void update(float deltaTime);
    bool checkCollision(const Rectangle& rect1, const Rectangle& rect2) const;

private:
    std::vector<std::unique_ptr<Entity>> entities;
    EntityType map[height][width];

    // Helper methods
    Rectangle getEntityBounds(const Entity& entity) const;

    void updatePacmanPosition(Pacman &pacman, float deltaTime);

    void checkPacmanCollisions(Pacman &pacman);

    void updateGhostPosition(Ghost &ghost, float deltaTime);

    void checkGhostCollisions(Ghost &ghost);
};

} // namespace Logic

#endif // WORLD_HPP