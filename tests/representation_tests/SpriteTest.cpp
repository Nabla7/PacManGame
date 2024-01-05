// SpriteTest.cpp
#include "EntityView.hpp"
#include "factories/ConcreteEntityFactory.hpp"
#include <SFML/Graphics.hpp>

namespace Representation {

void testSprites() {
    // Initialize the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sprite Test");

    // Create a factory
    Logic::PacmanGameEntityFactory factory;

    // Use the factory to create entities
    std::unique_ptr<Logic::Entity> coin = factory.createCoin();
    std::unique_ptr<Logic::Entity> fruit = factory.createFruit();
    std::unique_ptr<Logic::Entity> ghost = factory.createGhost();
    std::unique_ptr<Logic::Entity> pacman = factory.createPacman();

    // Set the positions of the entities
    coin->position.x = -0.5f;
    fruit->position.x = -0.25f;
    ghost->position.x = 0.0f;
    pacman->position.x = 0.25f;

    // Create a Camera object
    Camera camera(window.getSize().x, window.getSize().y);

    // Path to the texture file
    std::string textureFilePath = "assets/images/Sprites.png";

    // Create an EntityView for each entity
    EntityView coinView(window, *coin, camera, textureFilePath);
    EntityView fruitView(window, *fruit, camera, textureFilePath);
    EntityView ghostView(window, *ghost, camera, textureFilePath);
    EntityView pacmanView(window, *pacman, camera, textureFilePath);

    // Loop until the window is closed
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw each sprite
        coinView.draw();
        fruitView.draw();
        ghostView.draw();
        pacmanView.draw();

        window.display();
    }
}

} // namespace Representation