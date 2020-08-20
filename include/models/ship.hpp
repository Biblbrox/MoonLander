#ifndef MOONLANDER_SHIP_HPP
#define MOONLANDER_SHIP_HPP

#include <iostream>
#include <texture.hpp>
#include <sprite.hpp>
#include <camera.hpp>
#include "types.hpp"

using Utils::Rect;

class Ship {
public:
    explicit Ship(Camera* camera, Sprite *tex, GLfloat vel_x = 0, GLfloat vel_y = 0);
    virtual ~Ship();

    // Coordinates methods
    GLfloat getX() const;
    GLfloat getY() const;
    Utils::Point getCoords() const;

    void setX(GLfloat x);
    void setY(GLfloat y);
    void setCoords(Utils::Point coords);

    void addX(GLfloat x);
    void addY(GLfloat y);
    void addCoords(Utils::Point coords);

    void render(MoonLanderProgram& program, Camera& camera);

    // Angle methods
    void rotate(double angle);
    double getAngle() const;

    // Velocity methods
    GLfloat getVelX() const;
    GLfloat getVelY() const;
    Utils::Point getVel() const;

    void setVelX(GLfloat vel);
    void setVelY(GLfloat vel);
    void setVel(Utils::Point vel);

    void addVel(Utils::Point vel);
    void addVelX(GLfloat vel);
    void addVelY(GLfloat vel);

    void setVelRot(GLfloat vel);
    void addVelRot(GLfloat vel);
    GLfloat getVelRot() const;

    bool enginesOn() const;
    void turnEngines();
    bool isLanded() const;
    void turnLanded();

    void setSprite(unsigned int idx);
    unsigned int getSpriteIdx() const;

private:
    Utils::Point vel;
    GLfloat vel_rot;

    double angle;
    bool engines_on;
    bool landed;
    unsigned int cur_sprite;

    Camera* camera;
    Sprite* texture;
};

#endif // MOONLANDER_SHIP_HPP