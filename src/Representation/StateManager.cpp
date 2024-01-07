#include "GameState.hpp"

namespace Representation {

    void StateManager::pushState(std::unique_ptr<State> state) {
        states.push(std::move(state));
    }

    void StateManager::popState() {
        if (!states.empty()) {
            states.pop();
        }
    }

    State* StateManager::getCurrentState() const {
        if (!states.empty()) {
            return states.top().get();
        }
        return nullptr;
    }

} // namespace Representation
