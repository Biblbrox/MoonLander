#include <iostream>
#include <texture.hpp>
#include <imagetexture.h>
#include "types.hpp"
#include "models/model.hpp"

class Ship: public Model {
public:
    explicit Ship(ImageTexture *tex, SDL_Rect *clip = nullptr,
            v_type vel_x = 0, v_type vel_y = 0);
    virtual ~Ship();

    int getX() const;
    int getY() const;
    SDL_Rect getCoords() const;

    void render(Renderer &renderer) override;

    void setX(int x);
    void setY(int y);
    void setCoords(SDL_Rect coords);

    v_type getVelX() const;
    v_type getVelY() const;
    Velocity getVel() const;

    void setVelX(v_type vel);
    void setVelY(v_type vel);
    void setVel(Velocity vel);

    void addVelX(v_type vel);
    void addVelY(v_type vel);
    void addVel(Velocity vel);

private:
    v_type vel_x;
    v_type vel_y;
};