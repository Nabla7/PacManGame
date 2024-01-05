// EntityView.cpp
#include "EntityView.hpp"
#include "Camera.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

namespace Representation {

EntityView::EntityView(sf::RenderWindow& window, const Logic::Entity& entity, int screenWidth, int screenHeight)
    : window_(window), entity_(entity), camera_(screenWidth, screenHeight) {
    // Load the sprite sheet
    sf::Texture texture;
    if (!texture.loadFromFile("assets/images/Sprites.png")) {
        // Print an error message and terminate the program
        std::cerr << "Failed to load image 'assets/images/Sprites.png'. Program will terminate.\n";
        std::exit(EXIT_FAILURE);
    }

    // Select the appropriate sprite based on the entity type
    sf::IntRect rect;
    switch (entity.getType()) {
        case Logic::EntityType::Coin:
            rect = sf::IntRect(401, 304, 50, 50);
            break;
        case Logic::EntityType::Fruit:
            rect = sf::IntRect(551, 554, 50, 50);
            break;
        case Logic::EntityType::Ghost:
            rect = sf::IntRect(1, 4, 50, 50);
            break;
        case Logic::EntityType::Pacman:
            rect = sf::IntRect(851, 54, 50, 50);
            break;
        // Add cases for other entity types if needed
        default:
            // Handle unknown entity type
            break;
    }

    // Set the sprite's texture and texture rect
    sprite_.setTexture(texture);
    sprite_.setTextureRect(rect);
}

void EntityView::draw() {
    // Get the entity's position
    Logic::Entity::Position pos = entity_.getPosition();

    // Project the entity's position to pixel coordinates
    auto [pixelX, pixelY] = camera_.projectPosition(pos.x, pos.y);

    // Get the entity's size
    auto [width, height] = entity_.getSize();

    // Project the entity's size to pixel coordinates
    auto [pixelWidth, pixelHeight] = camera_.projectSize(width, height);

    // Set the sprite's position and scale
    sprite_.setPosition(pixelX, pixelY);
    sprite_.setScale(pixelWidth / sprite_.getLocalBounds().width, pixelHeight / sprite_.getLocalBounds().height);

    // Draw the sprite
    window_.draw(sprite_);
}

} // namespace Representation
