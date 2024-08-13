#include "GameState.hpp"
#include "Game.hpp"
#include "WorldView.hpp"

namespace Representation {

    void MenuState::handleInput(Game& game, Logic::GameAction action) {
        if (action == Logic::GameAction::Quit) {
            game.getWindow().close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            game.getStateManager().pushState(std::make_unique<LevelState>(game));
        }
    }

    void MenuState::update(Game& game, double deltaTime) {
        // Update menu logic, like animations or menu selection effects.
    }

    void MenuState::render(Game& game) {
        auto& window = game.getWindow();

        sf::Text menuText;
        sf::Font font;
        if (font.loadFromFile("assets/font/Pixeboy.ttf")) {
            menuText.setFont(font);
            menuText.setString("Press Enter to Start");
            menuText.setCharacterSize(24);
            menuText.setFillColor(sf::Color::White);
            menuText.setPosition(window.getSize().x / 2 - menuText.getGlobalBounds().width / 2, 50);
            window.draw(menuText);

            // Render high scores
            sf::Text highScoreText;
            highScoreText.setFont(font);
            highScoreText.setCharacterSize(18);
            highScoreText.setFillColor(sf::Color::Yellow);
            highScoreText.setString("High Scores");
            highScoreText.setPosition(window.getSize().x / 2 - highScoreText.getGlobalBounds().width / 2, 100);
            window.draw(highScoreText);

            const auto& highScores = game.getHighScoreManager().getHighScores();
            for (size_t i = 0; i < highScores.size(); ++i) {
                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setCharacterSize(16);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setString(highScores[i].name + ": " + std::to_string(highScores[i].score) + " (Level " + std::to_string(highScores[i].level) + ")");
                scoreText.setPosition(window.getSize().x / 2 - scoreText.getGlobalBounds().width / 2, 140 + i * 30);
                window.draw(scoreText);
            }
        }
    }

} // namespace Representation