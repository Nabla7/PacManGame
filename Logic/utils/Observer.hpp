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
        void attach(const std::shared_ptr<IObserver>& observer) {
            observers.push_back(observer);
        }

        void notify(EntityType entityType) {
            observers.erase(
                    std::remove_if(observers.begin(), observers.end(),
                                   [](const std::weak_ptr<IObserver>& weakObs) {
                                       return weakObs.expired();
                                   }),
                    observers.end());

            for (auto& weakObs : observers) {
                if (auto observer = weakObs.lock()) {
                    observer->onNotify(entityType);
                }
            }
        }

    private:
        std::vector<std::weak_ptr<IObserver>> observers;
    };



    class Score : public IObserver {
    public:
        Score() : totalScore(0), coinValue(10), timeSinceLastDecay(0),
                  eatenCoins(0), totalCoins(0), eatenFruits(0), totalFruits(0) {}

        void onNotify(EntityType entityType) override {
            switch (entityType) {
                case EntityType::Coin:
                    totalScore += coinValue;
                    eatenCoins++;
                    std::cout << "Coin collected. Score: " << totalScore << ", Coins eaten: " << eatenCoins << "/" << totalCoins << std::endl;
                    break;
                case EntityType::Fruit:
                    totalScore += 30;
                    eatenFruits++;
                    std::cout << "Fruit eaten. Score: " << totalScore << ", Fruits eaten: " << eatenFruits << "/" << totalFruits << std::endl;
                    break;
                case EntityType::Ghost:
                    totalScore += 60;
                    std::cout << "Ghost eaten. Score: " << totalScore << std::endl;
                    break;
                default:
                    break;
            }
        }
        
        void update(double deltaTime) {
            timeSinceLastDecay += deltaTime;
            if (timeSinceLastDecay >= 1.0) {
                int decayAmount = static_cast<int>(timeSinceLastDecay);

                // Decay total score
                int previousTotalScore = totalScore;
                totalScore = std::max(0, totalScore - decayAmount);

                // Decay coin value
                int previousCoinValue = coinValue;
                coinValue = std::max(1, coinValue - decayAmount);  // Minimum coin value is 1

                timeSinceLastDecay -= decayAmount;

                std::cout << "Score decayed from " << previousTotalScore << " to " << totalScore << std::endl;
                std::cout << "Coin value decayed from " << previousCoinValue << " to " << coinValue << std::endl;
            }
        }

        void resetEatenCoinsAndFruits() {
            eatenCoins = 0;
            eatenFruits = 0;
        }

        int getCurrentScore() const { return totalScore; }
        int getEatenCoins() const { return eatenCoins; }
        int getEatenFruits() const { return eatenFruits; }
        void setTotalCoins(int total) { totalCoins = total; }
        void setTotalFruits(int total) { totalFruits = total; }

    private:
        int totalScore;
        int coinValue;
        double timeSinceLastDecay;
        int eatenCoins;
        int totalCoins;
        int eatenFruits;
        int totalFruits;
    };

} // namespace Logic

#endif // OBSERVER_HPP
