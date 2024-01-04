// Entity.hpp
#ifndef ENTITY_HPP
#define ENTITY_HPP
namespace Logic {

class Entity {
public:
    virtual ~Entity() = default;

    // Common attributes for all entities
    struct Position {
        int x;
        int y;
    } position;

    enum class Direction {
        Up,
        Down,
        Left,
        Right
    } direction;

    int speed;
};

class Coin : public Entity {
    int value; // The value of the coin when collected
    public:
        void setValue(int v) { value = v; }
        int getValue() const { return value; }
};

class Fruit : public Entity {
    // Fruit-specific attributes
    int value; // The value of the fruit when collected
};

class Ghost : public Entity {
    // Ghost-specific attributes
    bool isScared; // Whether the ghost is currently scared
};

class Wall : public Entity {
    // Wall-specific attributes
    // Walls might not need any specific attributes
};

class Pacman : public Entity {
    int lives; // The number of lives left
    public:
        void setLives(int l) { lives = l; }
        int getLives() const { return lives; }
};

} // namespace Logic
#endif // ENTITY_HPP