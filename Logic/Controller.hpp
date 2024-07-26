// Logic/Controller.hpp
#ifndef LOGIC_CONTROLLER_HPP
#define LOGIC_CONTROLLER_HPP

#include "InputHandler.hpp"
#include "models/World.hpp"

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