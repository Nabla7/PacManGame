// Representation/InputHandler.hpp
#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include <SFML/Graphics.hpp>

namespace Representation {

enum class Action {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Pause,
    Quit,
    None
};

class InputHandler {
public:
    InputHandler(sf::RenderWindow& window) : window_(window) {}
    Action handleInput();

private:
    sf::RenderWindow& window_;
};

} // namespace Representation

#endif // INPUT_HANDLER_HPP