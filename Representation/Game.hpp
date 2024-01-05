// Game.hpp

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "InputHandler.hpp"
#include "EntityView.hpp"
#include "../Logic/models/World.hpp"
#include "../Logic/factories/ConcreteEntityFactory.hpp"

namespace Representation {

class Game {
public:
    Game();
    ~Game();

    void run();

    const Logic::World& getWorld() const { return world_; }

private:
    sf::RenderWindow window_;
    Logic::World world_;
    Logic::PacmanGameEntityFactory entityFactory_;
    std::vector<EntityView> entityViews_;
    InputHandler inputHandler_;

    void processInput();
    void update(float deltaTime);
    void render();
};

} // namespace Representation

#endif // GAME_HPP