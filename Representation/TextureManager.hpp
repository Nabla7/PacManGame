class TextureManager {
public:
    static TextureManager& getInstance() {
        static TextureManager instance;
        return instance;
    }

    sf::Texture& getTexture(const std::string& filename) {
        auto it = textures_.find(filename);
        if (it == textures_.end()) {
            // Load texture if not already loaded
            sf::Texture texture;
            if (!texture.loadFromFile(filename)) {
                throw std::runtime_error("Failed to load texture: " + filename);
            }
            textures_[filename] = texture;
        }
        return textures_[filename];
    }

private:
    std::map<std::string, sf::Texture> textures_;
};