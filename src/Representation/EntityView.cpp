// EntityView.cpp
#include "EntityView.hpp"
#include "TextureManager.hpp"
#include <iostream>

namespace Representation {

    EntityView::EntityView(sf::RenderWindow& window,
                           Logic::Entity& entity,
                           Camera& camera,
                           const std::string& textureFile)
            : window_(window),
              entity_(entity),
              camera_(camera),
              texture_(TextureManager::getInstance().getTexture(textureFile)),
              frameTime_(0.1f),
              currentTime_(0.0f),
              currentFrame_(0),
              isAnimated_(false),
              isDying_(false)
    {
        setupAnimation();
        sprite_.setTexture(texture_);
        updateAnimation(0.0f);  // Set initial frame
        sprite_.setOrigin(sprite_.getLocalBounds().width / 2, sprite_.getLocalBounds().height / 2);
    }

    void EntityView::setupAnimation()
    {
        const int col_spacing = 51;
        const int row_spacing = 50;

        switch (entity_.getType()) {
            case Logic::EntityType::Pacman: {
                isAnimated_ = true;
                // Pac-Man moving animations (first 12 row elements from the 18th column)
                int startCol = 17 * col_spacing;
                animationFrames_[Logic::Entity::Direction::Right] = {
                        sf::IntRect(startCol, 0 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 1 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 2 * row_spacing, col_spacing, row_spacing)
                };
                animationFrames_[Logic::Entity::Direction::Down] = {
                        sf::IntRect(startCol, 3 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 4 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 5 * row_spacing, col_spacing, row_spacing)
                };
                animationFrames_[Logic::Entity::Direction::Left] = {
                        sf::IntRect(startCol, 6 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 7 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 8 * row_spacing, col_spacing, row_spacing)
                };
                animationFrames_[Logic::Entity::Direction::Up] = {
                        sf::IntRect(startCol, 9 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 10 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 11 * row_spacing, col_spacing, row_spacing)
                };

                // Pac-Man death animation (first 11 row elements from the 8th column)
                startCol = 7 * col_spacing;
                for (int i = 0; i < 11; ++i) {
                    deathAnimationFrames_.push_back(sf::IntRect(startCol, i * row_spacing, col_spacing, row_spacing));
                }
                break;
            }
            case Logic::EntityType::Ghost: {
                isAnimated_ = true;
                // Ghost animations (first 8 row elements from the first column)
                int startCol = 0;
                animationFrames_[Logic::Entity::Direction::Right] = {
                        sf::IntRect(startCol, 0 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 1 * row_spacing, col_spacing, row_spacing)
                };
                animationFrames_[Logic::Entity::Direction::Down] = {
                        sf::IntRect(startCol, 2 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 3 * row_spacing, col_spacing, row_spacing)
                };
                animationFrames_[Logic::Entity::Direction::Left] = {
                        sf::IntRect(startCol, 4 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 5 * row_spacing, col_spacing, row_spacing)
                };
                animationFrames_[Logic::Entity::Direction::Up] = {
                        sf::IntRect(startCol, 6 * row_spacing, col_spacing, row_spacing),
                        sf::IntRect(startCol, 7 * row_spacing, col_spacing, row_spacing)
                };
                break;
            }
            case Logic::EntityType::Coin:
                sprite_.setTextureRect(sf::IntRect(401, 304, 50, 50));
                break;
            case Logic::EntityType::Fruit:
                sprite_.setTextureRect(sf::IntRect(551, 554, 50, 50));
                break;
            default:
                sprite_.setTextureRect(sf::IntRect(0, 0, 50, 50));  // Default frame
                break;
        }
    }

    void EntityView::update(float deltaTime)
    {
        if (isAnimated_) {
            updateAnimation(deltaTime);
        }
    }

    void EntityView::updateAnimation(float deltaTime)
    {
        if (!isAnimated_) return;

        currentTime_ += deltaTime;
        if (currentTime_ >= frameTime_) {
            currentTime_ = 0.0f;
            currentFrame_++;
        }

        if (auto pacman = dynamic_cast<const Logic::Pacman*>(&entity_)) {
            if (pacman->isDying) {
                isDying_ = true;
                if (currentFrame_ >= deathAnimationFrames_.size()) {
                    currentFrame_ = deathAnimationFrames_.size() - 1;  // Stay on last frame
                }
                sprite_.setTextureRect(deathAnimationFrames_[currentFrame_]);
            } else {
                isDying_ = false;
                currentFrame_ %= animationFrames_[pacman->direction_].size();
                sprite_.setTextureRect(animationFrames_[pacman->direction_][currentFrame_]);
            }
        }
        else if (auto ghost = dynamic_cast<const Logic::Ghost*>(&entity_)) {
            currentFrame_ %= animationFrames_[ghost->lockedDirection].size();
            sprite_.setTextureRect(animationFrames_[ghost->lockedDirection][currentFrame_]);
        }
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