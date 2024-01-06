#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <iostream>
#include <utility>

namespace Logic {

    enum class EntityType {
        Empty = 0,
        Wall,
        Coin,
        Fruit,
        Ghost,
        Pacman
    };
    


class Entity {
public:

    virtual ~Entity() = default;

    // Common attributes for all entities
    struct Position {
        float x;
        float y;
    } position;

    enum class Direction {
        Up,
        Down,
        Left,
        Right
    } direction;

    int speed = 3;

    // Pure virtual function for getting the type of the entity
    virtual EntityType getType() const = 0;

    // Get the position of the entity
    Position getPosition() const { return position; }

    // Get the size of the entity (placeholder implementation)
    virtual std::pair<float, float> getSize() const {
        return {1.0f, 1.0f};  // Placeholder values
    }

    virtual void move(Direction direction) {}

    static std::string DirectionToString(Direction direction) {
        switch (direction) {
            case Direction::Up: return "Up";
            case Direction::Down: return "Down";
            case Direction::Left: return "Left";
            case Direction::Right: return "Right";
            default: return "Unknown";
        }
    }
};

class Pacman : public Entity {
    int lives; // The number of lives left

public:
    void setLives(int l) { lives = l; }
    int getLives() const { return lives; }
    Direction direction_;
    EntityType getType() const override { return EntityType::Pacman; }
    void move(Direction direction) override {
        direction_ = direction;
        std::cout << "Pacman is now moving in direction: " << Entity::DirectionToString(direction) << std::endl;
}
};

class Coin : public Entity {
    int value; // The value of the coin when collected

public:
    void setValue(int v) { value = v; }
    int getValue() const { return value; }

    EntityType getType() const override { return EntityType::Coin; }
    std::pair<float, float> getSize() const override {
        return {0.5f, 0.5f};  // Size for coins
    }
};

class Fruit : public Entity {
    int value; // The value of the fruit when collected

public:
    EntityType getType() const override { return EntityType::Fruit; }
};

class Ghost : public Entity {
    bool isScared; // Whether the ghost is currently scared

public:
    EntityType getType() const override { return EntityType::Ghost; }
};

class Wall : public Entity {
public:
    EntityType getType() const override { return EntityType::Wall; }
    std::pair<float, float> getSize() const override {
        return {1.08f, 1.08f};  // Size for walls
    }
};

} // namespace Logic

#endif // ENTITY_HPP
