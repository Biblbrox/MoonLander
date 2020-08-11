#ifndef MOONLANDER_LEVEL_HPP
#define MOONLANDER_LEVEL_HPP

#include <SDL_render.h>
#include <vector>
#include "types.hpp"
#include "model.hpp"

class Level: public Model {
public:
    Level(Camera* camera);
    ~Level();
    void render(MoonLanderProgram& program) override;
    int renderSky();
    void setSurfaceType(SurfaceType surfaceType);
    bool hasCollision(Utils::Rect coord, GLfloat angle);

private:
    SurfaceType surfaceType;
    std::vector<Utils::Point> points;
    std::vector<Utils::Point> stars;
    int platforms_count;

    Camera* camera;
};


#endif //MOONLANDER_LEVEL_HPP
