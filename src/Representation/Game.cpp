// Representation/Game.cpp

#include "Game.hpp"
#include "Camera.hpp"
#include "utils/Stopwatch.hpp"

namespace Representation {

// Game.cpp
Game::Game()
    : window_(sf::VideoMode(20*32, 11*32), "Pacman Game"),
      inputHandler_(window_),
      world_(std::make_shared<Logic::PacmanGameEntityFactory>()),
      camera_(window_.getSize().x, window_.getSize().y),
      view_(sf::FloatRect(-200.f, -50.f, 1200.f, 500.f)){

    // Initialize world with the factory
    // Create an EntityView for each Entity in the World
    for (const auto& entity : world_.getEntities()) {
        entityViews_.emplace_back(window_, *entity, camera_, textureFilePath);
    }

        window_.setView(view_);

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
    sf::Clock clock;
    float timeSinceLastUpdate = 0.0f;
    const float TimePerFrame = 1.f / 60.f; // 60 updates per second

    while (window_.isOpen()) {
        processInput(); // Handle user input

        // Update game logic at a fixed time step
        timeSinceLastUpdate += clock.restart().asSeconds();
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            update(TimePerFrame); // Update game state
        }

        render(); // Render the frame
    }
}


void Game::processInput() {
    Action action = inputHandler_.handleInput();
    Logic::Pacman* pacman = world_.getPacman(); // Get Pacman from world
    if (pacman) { // Check if Pacman is not null
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