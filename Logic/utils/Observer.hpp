// Observer.hpp
#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "../models/Entity.hpp"
#include <vector>

namespace Logic {

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onNotify(EntityType entityType) = 0;
};

class Subject {
public:
    void attach(IObserver* observer) {
        observers.push_back(observer);
    }

    void notify(EntityType entityType) {
        for (IObserver* observer : observers) {
            observer->onNotify(entityType);
        }
    }

private:
    std::vector<IObserver*> observers;
};

class Score : public IObserver {
public:
    Score() : currentScore(0) {}

    void onNotify(EntityType entityType) override {
        if (entityType == EntityType::Coin) {
            currentScore += coinScore;
            std::cout << "Score added: " << coinScore << ". Current score: " << currentScore << std::endl;
        }
        // Add other conditions for different score updates
    }

    int getCurrentScore() const { return currentScore; }

    // Other methods for high scores, saving/loading scores, etc.

private:
    int currentScore;
    const int coinScore = 10; // Score value per coin
    // Add other score values for different entities
};

} // namespace Logic

#endif // OBSERVER_HPP
