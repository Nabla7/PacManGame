// WorldView.cpp
#include "WorldView.hpp"
#include <algorithm>

namespace Representation {

    // Representation/WorldView.cpp
    WorldView::WorldView(sf::RenderWindow& window, Logic::World& world, Camera& camera, const std::string& textureFilePath)
            : window_(window), world_(world), camera_(camera), textureFilePath_(textureFilePath)
    {
        update(0.0f);
    }

    void WorldView::draw() {
        for (auto& entityView : entityViews_) {
            entityView->draw();
        }
    }

    void WorldView::update(float deltaTime) {
        // Remove EntityViews for entities that no longer exist
        entityViews_.erase(
                std::remove_if(entityViews_.begin(), entityViews_.end(),
                               [this](const std::unique_ptr<EntityView>& view) {
                                   return std::find_if(world_.getEntities().begin(), world_.getEntities().end(),
                                                       [&view](const std::shared_ptr<Logic::Entity>& entity) {
                                                           return &view->getEntity() == entity.get();
                                                       }) == world_.getEntities().end();
                               }),
                entityViews_.end()
        );

        // Add EntityViews for new entities
        for (const auto& entity : world_.getEntities()) {
            if (std::find_if(entityViews_.begin(), entityViews_.end(),
                             [&entity](const std::unique_ptr<EntityView>& view) {
                                 return &view->getEntity() == entity.get();
                             }) == entityViews_.end()) {
                entityViews_.push_back(std::make_unique<EntityView>(window_, *entity, camera_, textureFilePath_));
            }
        }

        // Update existing EntityViews
        for (auto& entityView : entityViews_) {
            entityView->update(deltaTime);
        }
    }

} // namespace Representation