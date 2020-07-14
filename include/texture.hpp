#ifndef MOONLANDER_TEXTURE_HPP
#define MOONLANDER_TEXTURE_HPP

#include <string>
#include <SDL_render.h>
#include "freelable.hpp"
#include "renderer.hpp"

class Texture : public Freelable {
public:
    Texture(Renderer& renderer, const std::string& path);
    ~Texture();
    bool loadFromFile(const std::string& path);

    void renderTexture(int x, int y, SDL_Rect *clip);
    void renderTexture(SDL_Rect& dst,
                       SDL_Rect* clip = nullptr);

    void free() override;

    int getWidth() const;
    int getHeight() const;

    int getX() const;
    int getY() const;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width;
    int height;
    int x;
    int y;
};

#endif //MOONLANDER_TEXTURE_HPP
