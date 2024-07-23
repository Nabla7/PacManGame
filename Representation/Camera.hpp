// Camera.hpp
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Logic/models/World.hpp"


namespace Representation {

class Camera {
public:
    Camera(int screenWidth, int screenHeight)
        : screenWidth_(screenWidth), screenHeight_(screenHeight) {}

    std::pair<double, double> projectPosition(double x, double y) const {
        // Assuming world's center is at (0,0) and dimensions are normalized [-1, 1]
        double pixelX = (x + 1.0f) / 2.0f * screenWidth_ ; // Normalized X to [0, 1] then scale
        double pixelY = (y + 1.0f) / 2.0f * screenHeight_; // Normalized Y to [0, 1] then scale
        return {pixelX, pixelY};
    }



    std::pair<double, double> projectSize(double width, double height) const {
        // Calculate the aspect ratios
        double worldAspectRatio = static_cast<double>(Logic::World::width) / static_cast<double>(Logic::World::height);
        double screenAspectRatio = static_cast<double>(screenWidth_) / static_cast<double>(screenHeight_);

        // Adjust size based on aspect ratios
        double adjustedWidth = width;
        double adjustedHeight = height;

        if (screenAspectRatio > worldAspectRatio) {
            // Screen is wider than the world
            adjustedHeight *= screenAspectRatio / worldAspectRatio;
        } else {
            // Screen is taller than the world
            adjustedWidth *= worldAspectRatio / screenAspectRatio;
        }

        double pixelWidth = adjustedWidth * screenWidth_ / Logic::World::width;
        double pixelHeight = adjustedHeight * screenHeight_ / Logic::World::height;
        return {pixelWidth, pixelHeight};
    }




private:
    int screenWidth_;
    int screenHeight_;
};

} // namespace Representation

#endif // CAMERA_HPP