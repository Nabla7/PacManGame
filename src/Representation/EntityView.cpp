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
            rect = sf::IntRect(852, 55, 40, 40);
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
        Logic::Entity::Position pos = entity_.getPosition();
        auto [pixelX, pixelY] = camera_.projectPosition(pos.x, pos.y);
        auto [width, height] = entity_.getSize();
        auto [pixelWidth, pixelHeight] = camera_.projectSize(width, height);

        if (entity_.getType() == Logic::EntityType::Wall) {
            sf::RectangleShape wallRect(sf::Vector2f(pixelWidth, pixelHeight));
            wallRect.setPosition(pixelX, pixelY);
            wallRect.setFillColor(sf::Color::Blue);
            window_.draw(wallRect);
        } else {
            sprite_.setPosition(pixelX + pixelWidth / 2, pixelY + pixelHeight / 2);
            sprite_.setScale(pixelWidth / sprite_.getLocalBounds().width,
                             pixelHeight / sprite_.getLocalBounds().height);
            window_.draw(sprite_);
        }
    }


} // namespace Representation
