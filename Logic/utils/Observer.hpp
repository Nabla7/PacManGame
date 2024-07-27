// Observer.hpp
#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "../models/Entity.hpp"
#include <vector>
#include <cmath>

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
        Score() : currentScore(0), timeSinceLastCoin(0), baseScorePerCoin(10), maxScorePerCoin(50), scoreDecayRate(1) {}

        void onNotify(EntityType entityType) override {
            if (entityType == EntityType::Coin) {
                int scoreForCoin = getScoreForCoin();
                currentScore += scoreForCoin;
                std::cout << "Score added: " << scoreForCoin << ". Current score: " << currentScore << std::endl;
            } else if (entityType == EntityType::Fruit) {
                currentScore += 100;
                std::cout << "Fruit eaten. Score: " << currentScore << std::endl;
            } else if (entityType == EntityType::Ghost) {
                currentScore += 200;
                std::cout << "Ghost eaten. Score: " << currentScore << std::endl;
            }
        }

        void update(double deltaTime) {
            currentScore = std::max(0, currentScore - static_cast<int>(scoreDecayRate * deltaTime));
            timeSinceLastCoin += deltaTime;
        }

        int getCurrentScore() const { return currentScore; }

    private:
        int currentScore;
        double timeSinceLastCoin;
        int baseScorePerCoin;
        int maxScorePerCoin;
        double scoreDecayRate;

        int getScoreForCoin() {
            double factor = std::exp(-timeSinceLastCoin / 2.0);
            int score = baseScorePerCoin + static_cast<int>((maxScorePerCoin - baseScorePerCoin) * factor);
            timeSinceLastCoin = 0;
            return score;
        }
    };

} // namespace Logic

#endif // OBSERVER_HPP
