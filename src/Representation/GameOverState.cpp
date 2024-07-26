#include "GameState.hpp"
#include "Game.hpp"

namespace Representation {

    void GameOverState::handleInput(Game& game, Logic::GameAction action) {
        if (action == Logic::GameAction::Quit) {
            game.getWindow().close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            game.getStateManager().pushState(std::make_unique<MenuState>());
        }
    }

    void GameOverState::update(Game& game, double deltaTime) {
        // Update logic for GameOverState if necessary
    }

    void GameOverState::render(Game& game) {
        auto& window = game.getWindow();

        sf::Text gameOverText;
        sf::Font font;
        if (font.loadFromFile("assets/font/Pixeboy.ttf")) { // Load a font
            gameOverText.setFont(font);
            gameOverText.setString("Game Over! Press Enter to return to menu");
            gameOverText.setCharacterSize(24); // Set character size
            gameOverText.setFillColor(sf::Color::White);
            gameOverText.setPosition(window.getSize().x / 2, window.getSize().y / 2); // Center the text
            window.draw(gameOverText);
        }
    }

} // namespace Representation
