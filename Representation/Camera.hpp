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
            double pixelX = x * (screenWidth_ / static_cast<double>(Logic::World::width));
            double pixelY = y * (screenHeight_ / static_cast<double>(Logic::World::height));
            return {pixelX, pixelY};
        }

        std::pair<double, double> projectSize(double width, double height) const {
            double pixelWidth = width * (screenWidth_ / static_cast<double>(Logic::World::width));
            double pixelHeight = height * (screenHeight_ / static_cast<double>(Logic::World::height));
            return {pixelWidth, pixelHeight};
        }

    private:
        int screenWidth_;
        int screenHeight_;
    };

} // namespace Representation

#endif // CAMERA_HPP