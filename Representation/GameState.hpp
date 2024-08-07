// GameState.hpp
#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <stack>
#include <memory>
#include <SFML/Graphics.hpp>

namespace Representation {

// Forward declarations
class Game;
class State;
class MenuState;
class LevelState;
class PausedState;
class StateManager;
// State interface
class State {
public:
    virtual ~State() = default;
    virtual void handleInput(Game& game) = 0;
    virtual void update(Game& game, double deltaTime) = 0;
    virtual void render(Game& game) = 0;
};

// Concrete States
class MenuState : public State {
    // Implementation for MenuState
public:
    void handleInput(Game& game) override;
    void update(Game& game, double deltaTime) override;
    void render(Game& game) override;
};

class LevelState : public State {
    // Implementation for LevelState
public:
    void handleInput(Game& game) override;
    void update(Game& game, double deltaTime) override;
    void render(Game& game) override;
};

class PausedState : public State {
public:
    void handleInput(Game& game) override;
    void update(Game& game, double deltaTime) override;
    void render(Game& game) override;
};

class GameOverState : public State {
public:
    void handleInput(Game& game) override;
    void update(Game& game, double deltaTime) override;
    void render(Game& game) override;
};


// StateManager
class StateManager {
public:
    void pushState(std::unique_ptr<State> state);
    void popState();
    State* getCurrentState() const;

private:
    std::stack<std::unique_ptr<State>> states;
};

} // namespace Representation

#endif // GAME_STATE_HPP

