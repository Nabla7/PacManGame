// EntityView.hpp
#ifndef ENTITY_VIEW_HPP
#define ENTITY_VIEW_HPP

#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "../Logic/models/Entity.hpp"

namespace Representation {

class EntityView {
public:
    EntityView(sf::RenderWindow& window, const Logic::Entity& entity, int screenWidth, int screenHeight);

    void draw();

    std::pair<float, float> getProjectedPosition() const {
        return camera_.projectPosition(entity_.getPosition().x, entity_.getPosition().y);
    }

    const sf::Sprite& getSprite() const { return sprite_; }

private:
    sf::RenderWindow& window_;
    const Logic::Entity& entity_;
    sf::Sprite sprite_;
    sf::Texture texture_;
    Camera camera_;
};

} // namespace Representation

#endif // ENTITY_VIEW_HPP