// Camera.hpp
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Logic/models/World.hpp"


namespace Representation {

class Camera {
public:
    Camera(int screenWidth, int screenHeight)
        : screenWidth_(screenWidth), screenHeight_(screenHeight) {}

    std::pair<float, float> projectPosition(float x, float y) const {
        // Assuming world's center is at (0,0) and dimensions are normalized [-1, 1]
        float pixelX = (x + 1.0f) / 2.0f * screenWidth_ ; // Normalized X to [0, 1] then scale
        float pixelY = (y + 1.0f) / 2.0f * screenHeight_; // Normalized Y to [0, 1] then scale
        return {pixelX, pixelY};
    }



    std::pair<float, float> projectSize(float width, float height) const {
        // Calculate the aspect ratios
        float worldAspectRatio = static_cast<float>(Logic::World::width) / static_cast<float>(Logic::World::height);
        float screenAspectRatio = static_cast<float>(screenWidth_) / static_cast<float>(screenHeight_);

        // Adjust size based on aspect ratios
        float adjustedWidth = width;
        float adjustedHeight = height;

        if (screenAspectRatio > worldAspectRatio) {
            // Screen is wider than the world
            adjustedHeight *= screenAspectRatio / worldAspectRatio;
        } else {
            // Screen is taller than the world
            adjustedWidth *= worldAspectRatio / screenAspectRatio;
        }

        float pixelWidth = adjustedWidth * screenWidth_ / Logic::World::width;
        float pixelHeight = adjustedHeight * screenHeight_ / Logic::World::height;
        return {pixelWidth, pixelHeight};
    }




private:
    int screenWidth_;
    int screenHeight_;
};

} // namespace Representation

#endif // CAMERA_HPP