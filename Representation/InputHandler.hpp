// Representation/InputHandler.hpp
#ifndef REPRESENTATION_INPUT_HANDLER_HPP
#define REPRESENTATION_INPUT_HANDLER_HPP

#include <SFML/Graphics.hpp>
#include "../Logic/InputHandler.hpp"

namespace Representation {

    class InputHandler : public Logic::InputHandler {
    public:
        InputHandler(sf::RenderWindow& window) : window_(window) {}
        Logic::GameAction handleInput() override;

    private:
        sf::RenderWindow& window_;
    };

} // namespace Representation

#endif // REPRESENTATION_INPUT_HANDLER_HPP