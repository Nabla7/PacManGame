#include "GameState.hpp"
#include "Game.hpp"

namespace Representation {

    void LevelState::handleInput(Game& game) {
        auto& inputHandler = game.getInputHandler();
        auto action = inputHandler.handleInput();

        auto pacman = game.getWorld().getPacman();
        if (pacman) {
        switch (action) {
            case Action::MoveUp:
                pacman->setDesiredDirection(Logic::Entity::Direction::Up);
                break;
            case Action::MoveDown:
                pacman->setDesiredDirection(Logic::Entity::Direction::Down);
                break;
            case Action::MoveLeft:
                pacman->setDesiredDirection(Logic::Entity::Direction::Left);
                break;
            case Action::MoveRight:
                pacman->setDesiredDirection(Logic::Entity::Direction::Right);
                break;
            case Action::Pause:
                game.getStateManager().pushState(std::make_unique<PausedState>());
                break;
            case Action::Quit:
                game.getWindow().close();
                break;
            default:
                break;
            }       
        }
    }

    void LevelState::update(Game& game, double deltaTime) {
        game.getWorld().update(deltaTime);

        // Add any additional update logic specific to LevelState
        // e.g., check for win/lose conditions
    }

    void LevelState::render(Game& game) {
        auto& window = game.getWindow();

        // Render the world and its entities
        for (const auto& entity : game.getWorld().getEntities()) {
            EntityView view(window, *entity, game.getCamera(), game.getTextureFilePath());
            view.draw();
        }

        // Render the score
        auto& scoreText = game.getScoreText();
        scoreText.setString("Score: " + std::to_string(game.getWorld().getScore()));
        window.draw(scoreText);
    }

} // namespace Representation
