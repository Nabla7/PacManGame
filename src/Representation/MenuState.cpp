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
            menuText.setPosition(window.getSize().x / 2, window.getSize().y / 2);
            window.draw(menuText);
        }
    }


} // namespace Representation
