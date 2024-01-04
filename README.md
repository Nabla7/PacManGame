```plaintext
.PacManGame
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
│   ├── Game.hpp
│   └── InputHandler.hpp
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
│       ├── Game.cpp
│       └── InputHandler.cpp
└── tests
    ├── TestDriver.cpp
    ├── logic_tests
    │   └── utils_tests
    │       ├── RandomTest.cpp
    │       └── StopwatchTest.cpp
    └── representation_tests
```