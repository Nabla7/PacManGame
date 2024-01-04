// Representation/Game.cpp

#include "Game.hpp"
#include "utils/Stopwatch.hpp"

namespace Representation {

Game::Game()
        : window_(sf::VideoMode(800, 600), "Pacman Game"),
          inputHandler_(window_),
          world_(std::make_shared<Logic::PacmanGameEntityFactory>()) { // Initialize world with the factory
    // Additional initialization if needed
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

void Game::render() {
    window_.clear();
    // Draw the world
    window_.display();
}

} // namespace Representation