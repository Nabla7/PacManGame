// Game.hpp

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "InputHandler.hpp"
#include "../Logic/models/World.hpp"
#include "../Logic/factories/ConcreteEntityFactory.hpp"

namespace Representation {

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    sf::RenderWindow window_;
    Logic::World world_;
    Logic::PacmanGameEntityFactory entityFactory_;
    InputHandler inputHandler_;

    void processInput();
    void update(float deltaTime);
    void render();
};

} // namespace Representation

#endif // GAME_HPP