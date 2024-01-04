// Representation/InputHandler.cpp
#include "InputHandler.hpp"
#include <iostream>

namespace Representation {

Action InputHandler::handleInput() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            std::cout << "Escape key pressed or window closed. Quitting...\n";
            return Action::Quit;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            std::cout << "P key pressed. Pausing...\n";
            return Action::Pause;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            std::cout << "W key pressed. Moving up...\n";
            return Action::MoveUp;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            std::cout << "S key pressed. Moving down...\n";
            return Action::MoveDown;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            std::cout << "A key pressed. Moving left...\n";
            return Action::MoveLeft;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            std::cout << "D key pressed. Moving right...\n";
            return Action::MoveRight;
        }
    }
    return Action::None;
}

} // namespace Representation