// Representation/Game.hpp
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "../Representation/InputHandler.hpp"
#include "EntityView.hpp"
#include "../Logic/models/World.hpp"
#include "../Logic/factories/ConcreteEntityFactory.hpp"
#include "../Logic/Controller.hpp"
#include "Camera.hpp"
#include "GameState.hpp"

namespace Representation {

    class Game : public Logic::Controller {
    public:
        Game();
        ~Game();

        void run();

        // Controller interface implementation
        void update(double deltaTime) override;
        void handleInput(Logic::GameAction action) override;
        void resetGame();
        Logic::World& getWorld() override { return world_; }

        // Accessor methods
        sf::RenderWindow& getWindow();
        InputHandler& getInputHandler();
        StateManager& getStateManager();
        Camera& getCamera() { return camera_; }
        const std::string& getTextureFilePath() const { return textureFilePath; }
        sf::Text& getScoreText() { return scoreText; }

    private:
        sf::RenderWindow window_;
        sf::View view_;
        Logic::World world_;
        Logic::PacmanGameEntityFactory entityFactory_;
        std::vector<EntityView> entityViews_;
        Representation::InputHandler inputHandler_;
        Camera camera_;
        sf::Font font;
        sf::Text scoreText;

        StateManager stateManager_;

        void render();

        std::string textureFilePath = "assets/images/Sprites.png";
    };

} // namespace Representation

#endif // GAME_HPP