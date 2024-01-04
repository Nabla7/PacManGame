// include/utils/Stopwatch.hpp
#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <chrono>

namespace utils {

class Stopwatch {
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    static constexpr double maxFrameRate = 60.0; // Maximum frame rate
    static constexpr double maxFrameTime = 1.0 / maxFrameRate; // Maximum time per frame

public:
    static Stopwatch& getInstance();
    void start();
    void stop();
    double getElapsedTime() const;
    void capFrameRate() const;
    static double getMaxFrameTime();

private:
    Stopwatch() = default;
    TimePoint start_time_;
    TimePoint end_time_;
    bool running_ = false;
    // Other non-public members...
};

} // namespace utils

#endif // STOPWATCH_HPP