// Representation/LevelState.cpp
#include "GameState.hpp"
#include "Game.hpp"
#include "WorldView.hpp"

namespace Representation {

    LevelState::LevelState(Game& game) : worldView(std::make_unique<WorldView>(game.getWindow(), game.getWorld(), game.getCamera(), game.getTextureFilePath())) {
        if (font.loadFromFile("assets/font/Pixeboy.ttf")) {
            levelText.setFont(font);
            levelText.setCharacterSize(48);
            levelText.setFillColor(sf::Color::White);
        }
    }

    void LevelState::handleInput(Game& game, Logic::GameAction action) {
        auto pacman = game.getWorld().getPacman();
        if (pacman) {
            switch (action) {
                case Logic::GameAction::MoveUp:
                    pacman->setDesiredDirection(Logic::Entity::Direction::Up);
                    break;
                case Logic::GameAction::MoveDown:
                    pacman->setDesiredDirection(Logic::Entity::Direction::Down);
                    break;
                case Logic::GameAction::MoveLeft:
                    pacman->setDesiredDirection(Logic::Entity::Direction::Left);
                    break;
                case Logic::GameAction::MoveRight:
                    pacman->setDesiredDirection(Logic::Entity::Direction::Right);
                    break;
                case Logic::GameAction::Pause:
                    game.getStateManager().pushState(std::make_unique<PausedState>());
                    break;
                case Logic::GameAction::Quit:
                    game.getWindow().close();
                    break;
                default:
                    break;
            }
        }
    }

    void LevelState::update(Game& game, double deltaTime) {
        auto& world = game.getWorld();
        world.update(deltaTime);

        worldView->update();

        std::cout << "Pacman lives: " << world.getPacman()->getLives() << std::endl;
        std::cout << "Coins left: " << (world.totalCoins - world.getEatenCoins()) << std::endl;
        std::cout << "Fruits left: " << (world.totalFruits - world.getEatenFruits()) << std::endl;

        if (showingLevelText) {
            levelDisplayTimer -= deltaTime;
            if (levelDisplayTimer <= 0) {
                showingLevelText = false;
            }
        }

        if (world.allCoinsAndFruitsEaten()) {
            world.incrementLevel();
            world.prepareNextLevel();

            levelText.setString("Level " + std::to_string(world.getCurrentLevel()));
            levelText.setPosition(
                    game.getWindow().getSize().x / 2 - levelText.getGlobalBounds().width / 2,
                    game.getWindow().getSize().y / 2 - levelText.getGlobalBounds().height / 2
            );
            showingLevelText = true;
            levelDisplayTimer = 2.0f;
        }

        if (world.getPacman()->getLives() <= 0) {
            game.getStateManager().pushState(std::make_unique<GameOverState>());
        }
    }

    void LevelState::render(Game& game) {
        auto& window = game.getWindow();

        worldView->draw();

        // Render the score
        auto& scoreText = game.getScoreText();
        scoreText.setString("Score: " + std::to_string(game.getWorld().getScore()));
        window.draw(scoreText);

        // Render level text if needed
        if (showingLevelText) {
            window.draw(levelText);
        }
    }

} // namespace Representation