// Game.hpp

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "InputHandler.hpp"
#include "EntityView.hpp"
#include "../Logic/models/World.hpp"
#include "../Logic/factories/ConcreteEntityFactory.hpp"
#include "Camera.hpp"
#include "GameState.hpp"

namespace Representation {

class Game {
public:
    Game();
    ~Game();

    void run();

    const Logic::World& getWorld() const { return world_; }

private:
    sf::RenderWindow window_;
    sf::View view_;
    Logic::World world_;
    Logic::PacmanGameEntityFactory entityFactory_;
    std::vector<EntityView> entityViews_;
    InputHandler inputHandler_;
    Camera camera_;
    sf::Font font;
    sf::Text scoreText;
    StateManager stateManager_;

    void processInput();
    void update(float deltaTime);
    void render();

    std::string textureFilePath = "assets/images/Sprites.png";
};

} // namespace Representation

#endif // GAME_HPP