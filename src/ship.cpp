#include <GL/glew.h>
#include <models/ship.hpp>

Ship::Ship(Sprite *tex, GLfloat vel_x, GLfloat vel_y) :
        Model(tex, clip), engines_on(false), angle(0.),
        vel_rot(0), cur_sprite(0), landed(false)
{
    vel.x = vel_x;
    vel.y = vel_y;
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

Utils::Point Ship::getVel() const
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

void Ship::setVel(Utils::Point v)
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

void Ship::addVel(Utils::Point v)
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
    texture->render(getX(), getY(), cur_sprite, angle);
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

void Ship::setSprite(unsigned int idx)
{
    cur_sprite = idx;
}

unsigned int Ship::getSpriteIdx() const
{
    return cur_sprite;
}

bool Ship::isLanded() const
{
    return landed;
}

void Ship::turnLanded()
{
    landed = !landed;
}
