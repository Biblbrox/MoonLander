#ifndef MOONLANDER_TEXTURE_HPP
#define MOONLANDER_TEXTURE_HPP

#include <string>
#include <SDL_render.h>
#include "freelable.hpp"
#include "renderer.hpp"

class Texture : public Freelable {
public:
    explicit Texture(Renderer& renderer);
    //TODO: create another Texture class for ttf. Create base Texture, TextureImage, TextureText
    virtual ~Texture();

    virtual void renderTexture(int x, int y, SDL_Rect *clip);
    virtual void renderTexture(SDL_Rect& dst,
                       SDL_Rect* clip);

    virtual void free() override;

    virtual int getWidth() const;
    virtual int getHeight() const;

    virtual int getX() const;
    virtual int getY() const;

protected:
    SDL_Renderer* renderer;
    SDL_Texture* texture = nullptr;
    int width = 0;
    int height = 0;
    int x = 0;
    int y = 0;
};

#endif //MOONLANDER_TEXTURE_HPP
