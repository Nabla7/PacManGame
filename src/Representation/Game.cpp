// Representation/Game.cpp

#include "Game.hpp"
#include "Camera.hpp"
#include "utils/Stopwatch.hpp"

namespace Representation {

// Game.cpp
Game::Game()
    : window_(sf::VideoMode(800, 600), "Pacman Game"),
      inputHandler_(window_),
      world_(std::make_shared<Logic::PacmanGameEntityFactory>()) { // Initialize world with the factory
    // Create an EntityView for each Entity in the World
    int screenWidth = window_.getSize().x;
    int screenHeight = window_.getSize().y;
    for (const auto& entity : world_.getEntities()) {
        entityViews_.emplace_back(window_, *entity, screenWidth, screenHeight);
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
    switch (action) {
        case Action::MoveUp:
            // Translate to game logic
            break;
        case Action::MoveDown:
            // Translate to game logic
            break;
        case Action::MoveLeft:
            // Translate to game logic
            break;
        case Action::MoveRight:
            // Translate to game logic
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

void Game::update(float deltaTime) {
    // Update the world
    world_.update(deltaTime);
}



} // namespace Representation