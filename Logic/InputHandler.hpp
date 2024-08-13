// Logic/InputHandler.hpp
#ifndef LOGIC_INPUT_HANDLER_HPP
#define LOGIC_INPUT_HANDLER_HPP

namespace Logic {

    enum class GameAction {
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
        virtual ~InputHandler() = default;
        virtual GameAction handleInput() = 0;
    };

} // namespace Logic

#endif // LOGIC_INPUT_HANDLER_HPP