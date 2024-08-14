// EntityView.hpp
#ifndef ENTITY_VIEW_HPP
#define ENTITY_VIEW_HPP

#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "../Logic/models/Entity.hpp"
#include <vector>
#include <map>

namespace Representation {

    class EntityView {
    public:
        EntityView(sf::RenderWindow& window,
                   Logic::Entity& entity,
                   Camera& camera,
                   const std::string& textureFile);

        void draw();
        void update(float deltaTime);
        sf::IntRect getCurrentFrame() const; // New method

    private:
        sf::RenderWindow& window_;
        const Logic::Entity& entity_;
        sf::Sprite sprite_;
        sf::Texture& texture_;
        Camera& camera_;

        std::map<Logic::Entity::Direction, std::vector<sf::IntRect>> animationFrames_;
        std::vector<sf::IntRect> deathAnimationFrames_;
        float frameTime_;
        float currentTime_;
        int currentFrame_;
        bool isAnimated_;
        bool isDying_;

        void setupAnimation();
        void updateAnimation(float deltaTime);
    };

} // namespace Representation

#endif // ENTITY_VIEW_HPP