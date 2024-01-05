// Representation/Game.cpp

#include "Game.hpp"
#include "Camera.hpp"
#include "utils/Stopwatch.hpp"

namespace Representation {

// Game.cpp
Game::Game()
    : window_(sf::VideoMode(800, 600), "Pacman Game"),
      inputHandler_(window_),
      world_(std::make_shared<Logic::PacmanGameEntityFactory>()),
      camera_(window_.getSize().x, window_.getSize().y) { // Initialize world with the factory
    // Create an EntityView for each Entity in the World
    std::string textureFilePath = "assets/images/Sprites.png";
    for (const auto& entity : world_.getEntities()) {
        entityViews_.emplace_back(window_, *entity, camera_, textureFilePath);
    }
}

void Game::render() {
    window_.clear();
    // Draw each EntityView
    for (auto& entityView : entityViews_) {
        entityView.draw();
    }
    window_.display();
}

Game::~Game() {
    // Cleanup if necessary
}

void Game::run() {
    utils::Stopwatch& stopwatch = utils::Stopwatch::getInstance();
    while (window_.isOpen()) {
        stopwatch.start();

        processInput();
        update(stopwatch.getElapsedTime());
        render();

        stopwatch.stop();
        stopwatch.capFrameRate();
    }
}

void Game::processInput() {
    Action action = inputHandler_.handleInput();
    Logic::Pacman* pacman = world_.getPacman(); // Get Pacman from world
    if (pacman) { // Check if Pacman is not null
        switch (action) {
            case Action::MoveUp:
                pacman->move(Logic::Entity::Direction::Up);
                break;
            case Action::MoveDown:
                pacman->move(Logic::Entity::Direction::Down);
                break;
            case Action::MoveLeft:
                pacman->move(Logic::Entity::Direction::Left);
                break;
            case Action::MoveRight:
                pacman->move(Logic::Entity::Direction::Right);
                break;
            case Action::Pause:
                // Pause the game
                break;
            case Action::Quit:
                window_.close();
                break;
            default:
                break;
        }
    }
}

void Game::update(float deltaTime) {
    // Update the world
    world_.update(deltaTime);
}



} // namespace Representation