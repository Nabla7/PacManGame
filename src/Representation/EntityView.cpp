// EntityView.cpp
#include "EntityView.hpp"
#include "Camera.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureManager.hpp"

namespace Representation {

EntityView::EntityView(sf::RenderWindow& window, 
                        Logic::Entity& entity,  
                        Camera& camera,
                        const std::string& textureFile)
    :   window_(window), 
        entity_(entity), 
        camera_(camera),
        texture_(TextureManager::getInstance().getTexture(textureFile))
         {
    

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
    sprite_.setTexture(texture_);
    sprite_.setTextureRect(rect);
    sprite_.setOrigin(sprite_.getLocalBounds().width / 2, sprite_.getLocalBounds().height / 2);
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

        if (entity_.getType() == Logic::EntityType::Wall) {
            // For a wall (rectangle), position is the top-left corner
            sf::RectangleShape wallRect(sf::Vector2f(pixelWidth, pixelHeight));
            wallRect.setPosition(pixelX - pixelWidth / 2, pixelY - pixelHeight / 2);
            wallRect.setFillColor(sf::Color::Blue);
            window_.draw(wallRect);
        } else {
            // For a sprite, set the origin to its center
            sprite_.setOrigin(sprite_.getLocalBounds().width / 2, sprite_.getLocalBounds().height / 2);
            sprite_.setPosition(pixelX, pixelY);
            sprite_.setScale(pixelWidth / sprite_.getLocalBounds().width, pixelHeight / sprite_.getLocalBounds().height);
            window_.draw(sprite_);
        }
    }



} // namespace Representation
