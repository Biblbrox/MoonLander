#ifndef MOONLANDER_LEVEL_HPP
#define MOONLANDER_LEVEL_HPP

#include <SDL_render.h>
#include <vector>
#include "types.hpp"
#include "abstract/drawable.hpp"

class Level: public Drawable<Texture> {
public:
    explicit Level(Camera* camera);
    ~Level();
    void render(MoonLanderProgram& program) override;
    bool hasCollision(Utils::Rect coord, GLfloat angle);
    void extendToRight();
    void extendToLeft();

private:
    SurfaceType surfaceType;
    std::vector<Utils::Point> points;
    std::vector<Utils::Point> stars;

    Camera* camera;

    int points_count;
    int stars_count;
    const int frame_width = 2000;

    std::vector<Utils::Point> generate_lines(int initial_x);
    Utils::RandomUniform urand;
    int height_min;
    int height_max;
};


#endif //MOONLANDER_LEVEL_HPP
