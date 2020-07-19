#include <models/ship.hpp>

Ship::Ship(Sprite *tex, GLfloat vel_x, GLfloat vel_y) :
        Model(tex, clip), vel(vel_x, vel_y), engines_on(false), angle(0.),
        vel_rot(0)
{

}

Ship::~Ship()
{

}

GLfloat Ship::getVelX() const
{
    return vel.x;
}

GLfloat Ship::getVelY() const
{
    return vel.y;
}

Vector2D<GLfloat> Ship::getVel() const
{
    return vel;
}

void Ship::setVelX(GLfloat v)
{
    vel.x = v;
}

void Ship::setVelY(GLfloat v)
{
    vel.y = v;
}

void Ship::setVel(Vector2D<GLfloat> v)
{
    vel.x = v.x;
    vel.y = v.y;
}

void Ship::addVelY(GLfloat v)
{
    vel.y += v;
}

void Ship::addVelX(GLfloat v)
{
    vel.x +=v;
}

void Ship::addVel(Vector2D<GLfloat> v)
{
    vel.x += v.x;
    vel.y += v.y;
}

GLfloat Ship::getX() const
{
    return texture->getX();
}

GLfloat Ship::getY() const
{
    return texture->getY();
}

Utils::Point Ship::getCoords() const
{
    return {texture->getX(), texture->getY()};
}

void Ship::setX(GLfloat x)
{
    texture->setX(x);
}

void Ship::setY(GLfloat y)
{
    texture->setY(y);
}

void Ship::setCoords(Utils::Point coords)
{
    texture->setX(coords.x);
    texture->setY(coords.y);
}

void Ship::render()
{
    texture->render(getX(), getY(), engines_on ? 1 : 0, angle);
}

void Ship::rotate(double ang)
{
    angle += ang;
}

double Ship::getAngle() const
{
    return angle;
}

void Ship::addCoords(Utils::Point coords)
{
    texture->setX(texture->getX() + coords.x);
    texture->setY(texture->getY() + coords.y);
}

void Ship::addY(GLfloat y)
{
    texture->setY(texture->getY() + y);
}

void Ship::addX(GLfloat x)
{
    texture->setX(texture->getX() + x);
}

GLfloat Ship::getVelRot() const
{
    return vel_rot;
}

void Ship::setVelRot(GLfloat vel_)
{
    vel_rot = vel_;
}

void Ship::addVelRot(GLfloat vel_)
{
    vel_rot += vel_;
}

bool Ship::enginesOn() const
{
    return engines_on;
}

void Ship::turnEngines()
{
    engines_on = !engines_on;
}
