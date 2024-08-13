#include "GameState.hpp"
#include "Game.hpp"

namespace Representation {

    void GameOverState::handleInput(Game& game, Logic::GameAction action) {
        if (action == Logic::GameAction::Quit) {
            game.getWindow().close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            shouldTransitionToMenu = true;
        }
    }

    void GameOverState::update(Game& game, double deltaTime) {
        if (shouldTransitionToMenu) {
            game.resetGame();
            game.getStateManager().popState(); // Remove GameOverState
            game.getStateManager().pushState(std::make_unique<MenuState>());
        }
    }

    void GameOverState::render(Game& game) {
        auto& window = game.getWindow();

        sf::Text gameOverText;
        sf::Font font;
        if (font.loadFromFile("assets/font/Pixeboy.ttf")) {
            gameOverText.setFont(font);
            gameOverText.setString("Game Over! Press Enter to return to menu");
            gameOverText.setCharacterSize(24);
            gameOverText.setFillColor(sf::Color::White);
            gameOverText.setPosition(window.getSize().x / 2, window.getSize().y / 2);
            window.draw(gameOverText);
        }
    }

} // namespace Representation
