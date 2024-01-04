// include/utils/Random.hpp
#ifndef RANDOM_HPP
#define RANDOM_HPP
#include <random>

namespace utils {

class Random {
public:
    static Random& getInstance() {
        static Random instance;
        return instance;
    }

    int getInt(int lower, int upper) {
        std::uniform_int_distribution<int> dist(lower, upper);
        return dist(mt);
    }

    double getDouble(double lower, double upper) {
        std::uniform_real_distribution<double> dist(lower, upper);
        return dist(mt);
    }

private:
    std::mt19937 mt;

    // Private constructor and deleted functions to prevent multiple instances
    Random() : mt(std::random_device{}()) {}
    Random(const Random&) = delete;
    void operator=(const Random&) = delete;
};

};

#endif // RANDOM_HPP