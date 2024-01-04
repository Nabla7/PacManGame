// src/utils/Stopwatch.cpp
#include "utils/Stopwatch.hpp"
#include <thread>

using namespace utils;

Stopwatch& Stopwatch::getInstance() {
    static Stopwatch instance;
    return instance;
}

void Stopwatch::start() {
    start_time_ = Clock::now();
    running_ = true;
}

void Stopwatch::stop() {
    end_time_ = Clock::now();
    running_ = false;
}

double Stopwatch::getElapsedTime() const {
    auto end_time = running_ ? Clock::now() : end_time_;
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time_);
    return elapsed_time.count();
}

void Stopwatch::capFrameRate() const {
    double frameTime = getElapsedTime();
    if (frameTime < maxFrameTime) {
        std::this_thread::sleep_for(std::chrono::duration<double>(maxFrameTime - frameTime));
    }
}

double Stopwatch::getMaxFrameTime() {
    return maxFrameTime;
}
