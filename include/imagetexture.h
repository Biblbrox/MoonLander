#ifndef MOONLANDER_IMAGETEXTURE_H
#define MOONLANDER_IMAGETEXTURE_H

#include "texture.hpp"

class ImageTexture: public Texture {
public:
    explicit ImageTexture(Renderer* renderer, const std::string& path);
    bool load(const std::string& path);
};

#endif //MOONLANDER_IMAGETEXTURE_H
