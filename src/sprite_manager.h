#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "sprite.h"
#include <SFML/Graphics.hpp>

using SpriteID = size_t;

class SpriteManager {
public:
    sf::Texture& registerTexture(const std::string& filepath) {
        auto obj = mTextures.find(filepath);
        if (obj != mTextures.end()) {
            return obj->second; // Texture already registered
        }

        return mTextures.emplace(filepath, sf::Texture(filepath)).first->second;
    }

    template <typename... Args> 
    SpriteID createSprite(Args&&... args) {
        // Construct argument list and forward it to emplace_back
        mSprites.emplace_back(std::forward<Args>(args)...);

        return mSprites.size() - 1;
    }

    Sprite& getSprite(SpriteID id) {
        return mSprites.at(id);
    }

private:
    std::unordered_map<std::string, sf::Texture> mTextures;
    std::deque<Sprite> mSprites;
};

#endif  // SPRITE_MANAGER_H
