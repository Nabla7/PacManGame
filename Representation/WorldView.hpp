// Representation/WorldView.hpp
#ifndef WORLD_VIEW_HPP
#define WORLD_VIEW_HPP

#include <SFML/Graphics.hpp>
#include "../Logic/models/World.hpp"
#include "Camera.hpp"
#include "EntityView.hpp"

namespace Representation {

    class WorldView : public Logic::IObserver {
    public:
        WorldView(sf::RenderWindow& window, Logic::World& world, Camera& camera, const std::string& textureFilePath);
        void draw();
        void update();

        void onNotify(Logic::EntityType entityType) override {
            update();
        }

    private:
        sf::RenderWindow& window_;
        Logic::World& world_;
        Camera& camera_;
        std::vector<EntityView> entityViews_;
        std::string textureFilePath_;
    };

} // namespace Representation

#endif // WORLD_VIEW_HPP