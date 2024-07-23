#ifndef MAP_LOADER_HPP
#define MAP_LOADER_HPP

#include <string>
#include <vector>
#include "Entity.hpp"

namespace Logic {

    class MapLoader {
    public:
        static std::vector<std::vector<EntityType>> loadMap(const std::string& filepath);
    };

} // namespace Logic

#endif // MAP_LOADER_HPP
