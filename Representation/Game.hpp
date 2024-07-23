#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "InputHandler.hpp"
#include "EntityView.hpp"
#include "../Logic/models/World.hpp"
#include "../Logic/factories/ConcreteEntityFactory.hpp"
#include "Camera.hpp"
#include "GameState.hpp" // Include the state classes

namespace Representation {

    class Game {
    public:
        Game();
        ~Game();

        void run();

        // Accessor methods
        sf::RenderWindow& getWindow();
        InputHandler& getInputHandler();
        StateManager& getStateManager();
        Logic::World& getWorld();
        Camera& getCamera() { return camera_; }
        const std::string& getTextureFilePath() const { return textureFilePath; }
        sf::Text& getScoreText() { return scoreText; }

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

        StateManager stateManager_; // StateManager instance

        void processInput();
        void update(double deltaTime);
        void render();

        std::string textureFilePath = "assets/images/Sprites.png";
    };

} // namespace Representation

#endif // GAME_HPP
