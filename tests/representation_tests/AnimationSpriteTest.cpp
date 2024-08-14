// tests/representation_tests/AnimationSpriteTest.cpp
#include "EntityView.hpp"
#include "factories/ConcreteEntityFactory.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

namespace Representation {

    void testAnimationSprites() {
        // Initialize the window
        sf::RenderWindow window(sf::VideoMode(800, 600), "Animation Sprite Test");

        // Create a factory
        Logic::PacmanGameEntityFactory factory;

        // Use the factory to create entities
        std::shared_ptr<Logic::Pacman> pacman = std::dynamic_pointer_cast<Logic::Pacman>(factory.createPacman());
        std::shared_ptr<Logic::Ghost> ghost = std::dynamic_pointer_cast<Logic::Ghost>(factory.createGhost());

        // Set the positions of the entities
        pacman->position = {0.0, 0.0};
        ghost->position = {0.5, 0.0};

        // Create a Camera object
        Camera camera(window.getSize().x, window.getSize().y);

        // Path to the texture file
        std::string textureFilePath = "assets/images/Sprites.png";

        // Create an EntityView for each entity
        EntityView pacmanView(window, *pacman, camera, textureFilePath);
        EntityView ghostView(window, *ghost, camera, textureFilePath);

        sf::Clock clock;
        float totalTime = 0.0f;
        float lastDirectionChangeTime = 0.0f;

        // Loop until 5 seconds have passed
        while (totalTime < 30.0f) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            float deltaTime = clock.restart().asSeconds();
            totalTime += deltaTime;

            // Update animations
            pacmanView.update(deltaTime);
            ghostView.update(deltaTime);

            // Print current animation frames
            std::cout << "Time: " << totalTime << " seconds" << std::endl;
            std::cout << "Pacman frame: " << pacmanView.getCurrentFrame().left << ","
                      << pacmanView.getCurrentFrame().top << ","
                      << pacmanView.getCurrentFrame().width << ","
                      << pacmanView.getCurrentFrame().height << std::endl;

            std::cout << "Ghost frame: " << ghostView.getCurrentFrame().left << ","
                      << ghostView.getCurrentFrame().top << ","
                      << ghostView.getCurrentFrame().width << ","
                      << ghostView.getCurrentFrame().height << std::endl;

            window.clear();

            // Draw each sprite
            pacmanView.draw();
            ghostView.draw();

            window.display();

            // Change directions every second
            if (totalTime - lastDirectionChangeTime >= 1.0f) {
                pacman->direction_ = static_cast<Logic::Entity::Direction>((static_cast<int>(pacman->direction_) + 1) % 4);
                ghost->lockedDirection = static_cast<Logic::Entity::Direction>((static_cast<int>(ghost->lockedDirection) + 1) % 4);

                std::cout << "Changing direction - Pacman: " << static_cast<int>(pacman->direction_)
                          << ", Ghost: " << static_cast<int>(ghost->lockedDirection) << std::endl;

                lastDirectionChangeTime = totalTime;
            }
        }
    }

} // namespace Representation