#ifndef MOONLANDER_LEVEL_HPP
#define MOONLANDER_LEVEL_HPP

#include <SDL_render.h>
#include "types.hpp"
#include "renderer.hpp"
#include "model.hpp"

class Level: public Model {
public:
    Level();
    ~Level();
    void render(Renderer& renderer) override;
    int renderSky();
    void setSurfaceType(SurfaceType surfaceType);

private:
    SurfaceType surfaceType;
};


#endif //MOONLANDER_LEVEL_HPP
