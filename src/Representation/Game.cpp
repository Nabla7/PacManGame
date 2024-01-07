// Representation/Game.cpp
#include <iostream>
#include <filesystem>


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
      view_(sf::FloatRect(-200.f, -50.f, 1200.f, 500.f))
      {

        // Initialize world with the factory
        // Create an EntityView for each Entity in the World
        for (const auto& entity : world_.getEntities()) {
            entityViews_.emplace_back(window_, *entity, camera_, textureFilePath);
        }

        window_.setView(view_);

            std::cout << "Current path is " << std::filesystem::current_path() << '\n';
        try {
          if (!font.loadFromFile("assets/font/Pixeboy.ttf")) {
              throw std::runtime_error("Failed to load font from file");
          } else {
              std::cout << "Successfully loaded font from file" << std::endl;
          }
        } catch (const std::exception& e) {
          std::cerr << "Exception: " << e.what() << std::endl;
        }
        scoreText.setFont(font);
        scoreText.setCharacterSize(35); // Or any other suitable size
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(700, 0); // Top-left corner of the window
 }


void Game::render() {
    window_.clear();
    // Draw each EntityView
    for (auto& entityView : entityViews_) {
        entityView.draw();
    }

    // Update and draw score
    scoreText.setString("Score: " + std::to_string(world_.getScore()));
    window_.draw(scoreText);

    window_.display();
}

Game::~Game() {
    // Cleanup if necessary
}

void Game::run() {
    utils::Stopwatch& stopwatch = utils::Stopwatch::getInstance();
    stopwatch.start();

    while (window_.isOpen()) {
        processInput(); // Handle user input

        // Update game logic at a fixed time step
        while (stopwatch.getElapsedTime() > utils::Stopwatch::getMaxFrameTime()) {
            update(utils::Stopwatch::getMaxFrameTime()); // Update game state
            stopwatch.start(); // Restart the stopwatch
        }

        render(); // Render the frame
        stopwatch.capFrameRate(); // Cap the frame rate
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

    // Clear existing entity views
    entityViews_.clear();

    // Recreate entity views for updated entities in the world
    for (const auto& entity : world_.getEntities()) {
        entityViews_.emplace_back(window_, *entity, camera_, textureFilePath);
    }
}


} // namespace Representation