// ConcreteEntityFactory.hpp
#ifndef CONCRETE_ENTITY_FACTORY_HPP
#define CONCRETE_ENTITY_FACTORY_HPP

#include "EntityFactory.hpp"
#include "../models/Entity.hpp"

namespace Logic {

class PacmanGameEntityFactory : public EntityFactory {
public:
    std::shared_ptr<Entity> createCoin() override {
        return std::make_shared<Coin>();
    }

    std::shared_ptr<Entity> createFruit() override {
        return std::make_shared<Fruit>();
    }

    std::shared_ptr<Entity> createGhost() override {
        return std::make_shared<Ghost>();
    }

    std::shared_ptr<Entity> createWall() override {
        return std::make_shared<Wall>();
    }

    std::shared_ptr<Entity> createPacman() override {
        return std::make_shared<Pacman>();
    }
};

} // namespace Logic

#endif // CONCRETE_ENTITY_FACTORY_HPP