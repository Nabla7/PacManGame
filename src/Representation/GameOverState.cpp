#include "GameState.hpp"
#include "Game.hpp"

namespace Representation {

    GameOverState::GameOverState(int finalScore, int finalLevel)
            : finalScore_(finalScore), finalLevel_(finalLevel), nameEntered_(false), currentName_(""), shouldTransitionToMenu(false) {}

    void GameOverState::handleInput(Game& game, Logic::GameAction action) {
        if (action == Logic::GameAction::Quit) {
            game.getWindow().close();
        }

        if (!nameEntered_) {
            // Handle text input for name
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !currentName_.empty()) {
                game.getHighScoreManager().addScore(currentName_, finalScore_, finalLevel_);
                nameEntered_ = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) && !currentName_.empty()) {
                currentName_.pop_back();
            } else {
                sf::Event event;
                while (game.getWindow().pollEvent(event)) {
                    if (event.type == sf::Event::TextEntered) {
                        if (event.text.unicode < 128 && currentName_.length() < 10) {
                            currentName_ += static_cast<char>(event.text.unicode);
                        }
                    }
                }
            }
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
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
            gameOverText.setCharacterSize(24);
            gameOverText.setFillColor(sf::Color::White);

            if (!nameEntered_) {
                gameOverText.setString("Game Over!\nYour Score: " + std::to_string(finalScore_) +
                                       "\nLevel Reached: " + std::to_string(finalLevel_) +
                                       "\nEnter your name: " + currentName_);
            } else {
                gameOverText.setString("Game Over!\nPress Enter to return to menu");
            }

            gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2,
                                     window.getSize().y / 2 - gameOverText.getGlobalBounds().height / 2);
            window.draw(gameOverText);
        }
    }

} // namespace Representation