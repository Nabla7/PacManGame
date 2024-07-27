// Representation/GameState.hpp
#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <stack>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../Logic/InputHandler.hpp"

namespace Representation {

// Forward declarations
class Game;
class WorldView;

// State interface
class State {
public:
    virtual ~State() = default;
    virtual void handleInput(Game& game, Logic::GameAction action) = 0;
    virtual void update(Game& game, double deltaTime) = 0;
    virtual void render(Game& game) = 0;
};

class LevelState : public State {
private:
    sf::Text levelText;
    sf::Font font;
    float levelDisplayTimer = 2.0f;
    bool showingLevelText = true;
    std::unique_ptr<WorldView> worldView;

public:
    LevelState(Game& game);
    void handleInput(Game& game, Logic::GameAction action) override;
    void update(Game& game, double deltaTime) override;
    void render(Game& game) override;
};

// Concrete States
class MenuState : public State {
    // Implementation for MenuState
public:
    void handleInput(Game& game, Logic::GameAction action) override;
    void update(Game& game, double deltaTime) override;
    void render(Game& game) override;
};

class PausedState : public State {
public:
    void handleInput(Game& game, Logic::GameAction action) override;
    void update(Game& game, double deltaTime) override;
    void render(Game& game) override;
};

class GameOverState : public State {
public:
    GameOverState() : shouldTransitionToMenu(false) {}
    void handleInput(Game& game, Logic::GameAction action) override;
    void update(Game& game, double deltaTime) override;
    void render(Game& game) override;
private:
    bool shouldTransitionToMenu;
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

