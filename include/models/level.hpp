#ifndef MOONLANDER_LEVEL_HPP
#define MOONLANDER_LEVEL_HPP

#include <SDL_render.h>
#include <vector>
#include "types.hpp"
#include "model.hpp"

class Level: public Model {
public:
    Level();
    ~Level();
    void render() override;
    int renderSky();
    void setSurfaceType(SurfaceType surfaceType);

private:
    SurfaceType surfaceType;
    std::vector<SDL_Rect> points;
    int platforms_count;
    int platform_between;
};


#endif //MOONLANDER_LEVEL_HPP
