#include "GameState.hpp"
#include "Game.hpp"

namespace Representation {

    void MenuState::handleInput(Game& game) {
        // Handle menu input. For example, start the game or exit.
        auto& inputHandler = game.getInputHandler();
        auto action = inputHandler.handleInput();

        if (action == Action::Quit) {
            game.getWindow().close();
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            game.getStateManager().pushState(std::make_unique<LevelState>());
    }
    }

    void MenuState::update(Game& game, double deltaTime) {
        // Update menu logic, like animations or menu selection effects.
    }

    void MenuState::render(Game& game) {
        auto& window = game.getWindow();

        sf::Text menuText;
        sf::Font font;
        if (font.loadFromFile("assets/font/Pixeboy.ttf")) { // Load a font
            menuText.setFont(font);
            menuText.setString("Press Enter to Start");
            menuText.setCharacterSize(24); // Set character size
            menuText.setFillColor(sf::Color::White);
            menuText.setPosition(window.getSize().x / 2, window.getSize().y / 2); // Center the text
            window.draw(menuText);
    }
}


} // namespace Representation
