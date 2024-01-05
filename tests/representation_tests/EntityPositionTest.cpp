// EntityPositionTest.cpp
#include "Game.hpp"
#include "models/World.hpp"
#include "Camera.hpp" // Include this
#include <SFML/Graphics.hpp>

namespace Representation {

void testEntityPositions() {
    // Initialize the game world
    Logic::World world(std::make_shared<Logic::PacmanGameEntityFactory>());
    sf::RenderWindow window(sf::VideoMode(800, 600), "Entity Position Test");

    // Create a Camera object
    Camera camera(window.getSize().x, window.getSize().y);

    // Loop until the window is closed
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw each entity as a colored rectangle based on its type
        for (const auto& entity : world.getEntities()) {
            sf::RectangleShape rectangle;
            auto position = entity->getPosition();
            auto size = entity->getSize();

            // Convert game coordinates to window coordinates using the Camera object
            auto [pixelX, pixelY] = camera.projectPosition(position.x, position.y);
            rectangle.setPosition(pixelX, pixelY);

            auto [pixelWidth, pixelHeight] = camera.projectSize(size.first, size.second);
            rectangle.setSize(sf::Vector2f(pixelWidth, pixelHeight));

            // Set a distinct color for each entity type
            switch (entity->getType()) {
                case Logic::EntityType::Coin:
                    rectangle.setFillColor(sf::Color::Yellow);
                    break;
                case Logic::EntityType::Fruit:
                    rectangle.setFillColor(sf::Color::Red);
                    break;
                case Logic::EntityType::Ghost:
                    rectangle.setFillColor(sf::Color::Blue);
                    break;
                case Logic::EntityType::Pacman:
                    rectangle.setFillColor(sf::Color::Green);
                    break;
                // Add cases for other entity types if needed
                default:
                    rectangle.setFillColor(sf::Color::White); // Default color
                    break;
            }

            window.draw(rectangle);
        }

        window.display();
    }
}

} // namespace Representation