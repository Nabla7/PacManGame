#include "GameState.hpp"
#include "Game.hpp"

namespace Representation {

void PausedState::handleInput(Game& game) {
    auto& inputHandler = game.getInputHandler();
    auto action = inputHandler.handleInput();

    switch (action) {
        case Action::Pause:
            // Pop PausedState to resume game
            game.getStateManager().popState();
            break;
        case Action::Quit:
            // Close the game window
            game.getWindow().close();
            break;
        default:
            break;
    }
}

void PausedState::update(Game& game, float deltaTime) {
    // In paused state, we might not need to update anything.
    // However, you can add logic here if needed.
}

void PausedState::render(Game& game) {
    // Render pause menu or overlay if required.
    // For now, we won't render anything.
    // You can add a pause menu rendering here.
}

} // namespace Representation
