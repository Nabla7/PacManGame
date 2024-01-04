// EntityFactory.hpp
#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP
#include <memory> // Include for std::unique_ptr

namespace Logic {

class Entity;  // Forward declaration of Entity

class EntityFactory {
public:
    virtual ~EntityFactory() = default;

    virtual std::unique_ptr<Entity> createCoin() = 0;
    virtual std::unique_ptr<Entity> createFruit() = 0;
    virtual std::unique_ptr<Entity> createGhost() = 0;
    virtual std::unique_ptr<Entity> createWall() = 0;
    virtual std::unique_ptr<Entity> createPacman() = 0;
};

} // namespace Logic
#endif // ENTITY_FACTORY_HPP