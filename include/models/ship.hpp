#include <iostream>
#include <texture.hpp>
#include <imagetexture.h>
#include <2dvector.h>
#include "types.hpp"
#include "models/model.hpp"

using Utils::Rect;

class Ship: public Model {
public:
    explicit Ship(ImageTexture *tex, Rect *clip = nullptr,
            v_type vel_x = 0, v_type vel_y = 0);
    virtual ~Ship();

    // Coordinates methods
    int getX() const;
    int getY() const;
    SDL_Rect getCoords() const;

    void setX(int x);
    void setY(int y);
    void setCoords(SDL_Rect coords);

    void addX(int x);
    void addY(int y);
    void addCoords(SDL_Rect coords);

    void render() override;

    // Angle methods
    void rotate(double angle);
    double getAngle() const;

    // Velocity methods
    v_type getVelX() const;
    v_type getVelY() const;
    Vector2D<v_type> getVel() const;

    void setVelX(v_type vel);
    void setVelY(v_type vel);
    void setVel(Vector2D<v_type> vel);

    void addVelX(v_type vel);
    void addVelY(v_type vel);
    void addVel(Vector2D<v_type> vel);

    void setVelRot(v_type vel);
    void addVelRot(v_type vel);
    v_type getVelRot() const;

private:
    Vector2D<v_type> vel;
    v_type vel_rot;

    double angle;
    bool engines_on;
};