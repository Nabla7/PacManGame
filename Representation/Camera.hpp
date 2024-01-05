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

        // Convert to pixel coordinates
        float pixelX = (x + 1.25f) / 2.0f * screenWidth_;
        float pixelY = (0.40f - y) / 2.0f * screenHeight_; // Inverting Y to match screen coordinates
        return {pixelX, pixelY};
    }


    std::pair<float, float> projectSize(float width, float height) const {
        float pixelWidth = width / 2.0f * screenWidth_;
        float pixelHeight = height / 2.0f * screenHeight_;
        return {pixelWidth, pixelHeight};
    }
    
    

private:
    int screenWidth_;
    int screenHeight_;
};

} // namespace Representation

#endif // CAMERA_HPP