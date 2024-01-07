// Representation/Game.cpp
#include <iostream>
#include <filesystem>


#include "Game.hpp"
#include "Camera.hpp"
#include "utils/Stopwatch.hpp"
#include "GameState.hpp"

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

        // Initialize the state manager with the menu state
        stateManager_.pushState(std::make_unique<MenuState>());

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

    State* currentState = stateManager_.getCurrentState();
    if (currentState) {
        currentState->render(*this);
    }

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
    State* currentState = stateManager_.getCurrentState();
    if (currentState) {
        currentState->handleInput(*this);
    }
}

void Game::update(float deltaTime) {
    State* currentState = stateManager_.getCurrentState();
    if (currentState) {
        currentState->update(*this, deltaTime);
    }
}



sf::RenderWindow& Game::getWindow() {
    return window_;
}

InputHandler& Game::getInputHandler() {
    return inputHandler_;
}

StateManager& Game::getStateManager() {
    return stateManager_;
}

Logic::World& Game::getWorld() {
    return world_;
}


} // namespace Representation