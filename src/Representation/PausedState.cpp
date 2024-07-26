#include "GameState.hpp"
#include "Game.hpp"

namespace Representation {

void PausedState::handleInput(Game& game, Logic::GameAction action) {
    switch (action) {
        case Logic::GameAction::Pause:
            // Pop PausedState to resume game
            game.getStateManager().popState();
            break;
        case Logic::GameAction::Quit:
            // Close the game window
            game.getWindow().close();
            break;
        default:
            break;
    }
}

void PausedState::update(Game& game, double deltaTime) {
    // In paused state, we might not need to update anything.
    // However, you can add logic here if needed.
}

void PausedState::render(Game& game) {
    // Render pause menu or overlay if required.
    // For now, we won't render anything.
    // You can add a pause menu rendering here.
}

} // namespace Representation
