// src/Representation/InputHandler.cpp
#include "../Representation/InputHandler.hpp"
#include <SFML/Graphics.hpp>

namespace Representation {

    Logic::GameAction InputHandler::handleInput() {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                return Logic::GameAction::Quit;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                return Logic::GameAction::Pause;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                return Logic::GameAction::MoveUp;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                return Logic::GameAction::MoveDown;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                return Logic::GameAction::MoveLeft;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                return Logic::GameAction::MoveRight;
            }
        }
        return Logic::GameAction::None;
    }

} // namespace Representation