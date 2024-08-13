// WorldView.cpp
#include "WorldView.hpp"

namespace Representation {

    WorldView::WorldView(sf::RenderWindow& window, Logic::World& world, Camera& camera, const std::string& textureFilePath)
            : window_(window), world_(world), camera_(camera), textureFilePath_(textureFilePath)
    {
        update(0.0f);
    }

    void WorldView::draw() {
        for (auto& entityView : entityViews_) {
            entityView.draw();
        }
    }

    void WorldView::update(float deltaTime) {
        entityViews_.clear();
        for (const auto& entity : world_.getEntities()) {
            entityViews_.emplace_back(window_, *entity, camera_, textureFilePath_);
        }

        for (auto& entityView : entityViews_) {
            entityView.update(deltaTime);
        }
    }

} // namespace Representation