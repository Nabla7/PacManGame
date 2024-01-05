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
│       ├── Random.hpp
│       ├── Score.hpp
│       └── Stopwatch.hpp
├── README.md
├── Representation
│   ├── Camera.hpp
│   ├── EntityView.hpp
│   ├── Game.hpp
│   ├── InputHandler.hpp
│   └── TextureManager.hpp
├── assets
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
│       └── InputHandler.cpp
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