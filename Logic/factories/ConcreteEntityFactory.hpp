// ConcreteEntityFactory.hpp
#ifndef CONCRETE_ENTITY_FACTORY_HPP
#define CONCRETE_ENTITY_FACTORY_HPP

#include "EntityFactory.hpp"
#include "../models/Entity.hpp"

namespace Logic {

class PacmanGameEntityFactory : public EntityFactory {
public:
    std::unique_ptr<Entity> createCoin() override {
        return std::make_unique<Coin>();
    }

    std::unique_ptr<Entity> createFruit() override {
        return std::make_unique<Fruit>();
    }

    std::unique_ptr<Entity> createGhost() override {
        return std::make_unique<Ghost>();
    }

    std::unique_ptr<Entity> createWall() override {
        return std::make_unique<Wall>();
    }

    std::unique_ptr<Entity> createPacman() override {
        return std::make_unique<Pacman>();
    }
};

} // namespace Logic

#endif // CONCRETE_ENTITY_FACTORY_HPP