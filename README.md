# Technical Documentation for PacMan Game Codebase

## Overview

The PacMan Game codebase is a structured implementation of the arcade game using C++ and the SFML library. This documentation provides an overview of the architecture, design choices, and key components of the codebase.

```plaintext
.
├── CMakeLists.txt
├── Logic
│   ├── factories
│   │   ├── ConcreteEntityFactory.hpp
│   │   └── EntityFactory.hpp
│   ├── models
│   │   ├── Entity.hpp
│   │   └── World.hpp
│   └── utils
│       ├── Observer.hpp
│       ├── Random.hpp
│       └── Stopwatch.hpp
├── README.md
├── Representation
│   ├── Camera.hpp
│   ├── EntityView.hpp
│   ├── Game.hpp
│   ├── GameState.hpp
│   ├── InputHandler.hpp
│   └── TextureManager.hpp
├── assets
│   ├── font
│   │   ├── AGoblinAppears-o2aV.ttf
│   │   └── Pixeboy.ttf
│   └── images
│       └── Sprites.png
├── main.cpp
├── src
│   ├── Logic
│   │   ├── factories
│   │   ├── models
│   │   │   └── World.cpp
│   │   └── utils
│   │       └── Stopwatch.cpp
│   └── Representation
│       ├── Camera.cpp
│       ├── EntityView.cpp
│       ├── Game.cpp
│       ├── InputHandler.cpp
│       ├── LevelState.cpp
│       ├── MenuState.cpp
│       ├── PausedState.cpp
│       └── StateManager.cpp
└── tests
    ├── TestDriver.cpp
    ├── logic_tests
    │   ├── factories_tests
    │   ├── models_tests
    │   │   ├── EntityTest.cpp
    │   │   ├── WorldAddEntityTest.cpp
    │   │   └── WorldTest.cpp
    │   └── utils_tests
    │       ├── RandomTest.cpp
    │       └── StopwatchTest.cpp
    └── representation_tests
        ├── CameraProjectPositionTest.cpp
        ├── EntityPositionTest.cpp
        ├── GameTest.cpp
        └── SpriteTest.cpp
```

## Architecture

The architecture is divided into two main namespaces: `Logic` and `Representation`. This separation follows the Model-View-Controller (MVC) pattern, enhancing maintainability and scalability.

### Logic Namespace
- **Purpose**: Manages the game's logic, rules, and state.
- **Key Components**:
  - `Entity` and its derivatives (`Coin`, `Fruit`, `Ghost`, `Pacman`, `Wall`): Represent different game objects.
  - `EntityFactory` and `PacmanGameEntityFactory`: Implement the Factory Method pattern for creating `Entity` objects.
  - `World`: Holds the game state, including entities and their interactions.

### Representation Namespace
- **Purpose**: Handles the visual representation and user interaction.
- **Key Components**:
  - `Game`: Central class coordinating the game loop and state management.
  - `EntityView`: Represents the visual aspect of `Entity` objects.
  - `Camera`: Manages the projection of game objects onto the screen.
  - `State` and its derivatives (`MenuState`, `LevelState`, `PausedState`): Implement the State pattern for different game states.
  - `StateManager`: Manages the stack of game states.
  - `InputHandler`: Processes user input.

## Design Choices

### Separation of Concerns
- **Motivation**: Clear distinction between game logic and representation improves modularity and testability.
- **Implementation**: Two namespaces, `Logic` and `Representation`.

### Factory Method Pattern
- **Motivation**: Encapsulate the creation of `Entity` objects, allowing for easy extension.
- **Implementation**: `EntityFactory` interface and `PacmanGameEntityFactory` concrete implementation.

### State Pattern
- **Motivation**: Simplify state transitions and encapsulate state-specific behavior.
- **Implementation**: `State` interface with `MenuState`, `LevelState`, and `PausedState` classes.

### Singleton Pattern
- **Motivation**: Ensure a single instance of utility classes like `TextureManager` and `Stopwatch`.
- **Implementation**: Private constructors and static `getInstance` methods.

### Smart Pointers
- **Motivation**: Automatic memory management and prevention of memory leaks.
- **Implementation**: `std::unique_ptr` for managing `Entity` objects in `World` and `State` objects in `StateManager`.

### Observer Pattern
- **Motivation**: Decouple the score updating mechanism from the game logic.
- **Implementation**: `Observer` and `Subject` classes in `Logic` namespace.

### MVC Pattern
- **Motivation**: Separation of game logic (model), visual representation (view), and user input (controller).
- **Implementation**: Distinct `Logic` and `Representation` namespaces, with `Game` class acting as the controller.

### Exception Handling
- **Motivation**: Robust error handling, particularly for external resources like textures and fonts.
- **Implementation**: Use of `throw` and `try-catch` blocks in `TextureManager` and `Game` constructor.

### Efficient Object Handling
- **Motivation**: Minimize unnecessary copying of objects.
- **Implementation**: Use of references and pointers, especially in function parameters and return types.

## Conclusion

The PacMan Game codebase is designed with a focus on clear separation of concerns, robust design patterns, and efficient memory management. This structure not allows for easier maintenance and extension but also ensures a more stable and scalable application. The use of modern C++ features like smart pointers and exception handling further contributes to the codebase's robustness.