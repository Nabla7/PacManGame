// Representation/WorldView.cpp
#include "WorldView.hpp"

namespace Representation {

    // Representation/WorldView.cpp
    WorldView::WorldView(sf::RenderWindow& window, Logic::World& world, Camera& camera, const std::string& textureFilePath)
            : window_(window), world_(world), camera_(camera), textureFilePath_(textureFilePath)
    {
        update();
    }

    void WorldView::draw() {
        for (auto& entityView : entityViews_) {
            entityView.draw();
        }
    }

    // Representation/WorldView.cpp
    void WorldView::update() {
        entityViews_.clear();
        for (const auto& entity : world_.getEntities()) {
            entityViews_.emplace_back(window_, *entity, camera_, textureFilePath_);
        }
    }

} // namespace Representation