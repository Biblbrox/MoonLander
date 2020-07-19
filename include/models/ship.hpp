#include <iostream>
#include <texture.hpp>
#include <sprite.h>
#include <2dvector.h>
#include "types.hpp"
#include "models/model.hpp"

using Utils::Rect;

class Ship: public Model {
public:
    explicit Ship(Sprite *tex, GLfloat vel_x = 0, GLfloat vel_y = 0);
    virtual ~Ship();

    // Coordinates methods
    GLfloat  getX() const;
    GLfloat getY() const;
    Utils::Point getCoords() const;

    void setX(GLfloat x);
    void setY(GLfloat y);
    void setCoords(Utils::Point coords);

    void addX(GLfloat x);
    void addY(GLfloat y);
    void addCoords(Utils::Point coords);

    void render() override;

    // Angle methods
    void rotate(double angle);
    double getAngle() const;

    // Velocity methods
    GLfloat getVelX() const;
    GLfloat getVelY() const;
    Vector2D<GLfloat> getVel() const;

    void setVelX(GLfloat vel);
    void setVelY(GLfloat vel);
    void setVel(Vector2D<GLfloat> vel);

    void addVelX(GLfloat vel);
    void addVelY(GLfloat vel);
    void addVel(Vector2D<GLfloat> vel);

    void setVelRot(GLfloat vel);
    void addVelRot(GLfloat vel);
    GLfloat getVelRot() const;

    bool enginesOn() const;
    void turnEngines();

private:
    Vector2D<GLfloat> vel;
    GLfloat vel_rot;

    double angle;
    bool engines_on;
};