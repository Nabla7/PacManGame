// Logic/Controller.hpp
#ifndef LOGIC_CONTROLLER_HPP
#define LOGIC_CONTROLLER_HPP

#include "InputHandler.hpp"
#include "models/World.hpp"
/*
 * We create an interface from which world inherits such that we can update and handle input the world in one go.
 * World is inhabited by entities, all entities have unique methods for update and input.
 * This makes it easy and is good practice.
 * */
namespace Logic {

    class Controller {
    public:
        virtual ~Controller() = default;
        virtual void update(double deltaTime) = 0;
        virtual void handleInput(GameAction action) = 0;
        virtual World& getWorld() = 0;
    };

} // namespace Logic

#endif // LOGIC_CONTROLLER_HPP