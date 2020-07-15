#include <models/ship.hpp>

Ship::Ship(ImageTexture *tex, SDL_Rect *clip, v_type vel_x, v_type vel_y) :
        Model(tex, clip), vel_x(vel_x), vel_y(vel_y)
{

}

Ship::~Ship()
{

}

v_type Ship::getVelX() const
{
    return vel_x;
}

v_type Ship::getVelY() const
{
    return vel_y;
}

Velocity Ship::getVel() const
{
    return {vel_x, vel_y};
}

void Ship::setVelX(v_type vel)
{
    vel_x = vel;
}

void Ship::setVelY(v_type vel)
{
    vel_y = vel;
}

void Ship::setVel(Velocity vel)
{
    vel_x = vel.x;
    vel_y = vel.y;
}

void Ship::addVelY(v_type vel)
{
    vel_y += vel;
}

void Ship::addVelX(v_type vel)
{
    vel_x += vel;
}

void Ship::addVel(Velocity vel)
{
    vel_x += vel.x;
    vel_y += vel.y;
}

int Ship::getX() const
{
    return texture->getX();
}

int Ship::getY() const
{
    return texture->getY();
}

SDL_Rect Ship::getCoords() const
{
    return {texture->getX(), texture->getY()};
}

void Ship::setX(int x)
{
    texture->setX(x);
}

void Ship::setY(int y)
{
    texture->setY(y);
}

void Ship::setCoords(SDL_Rect coords)
{
    texture->setX(coords.x);
    texture->setY(coords.y);
}

void Ship::render(Renderer &renderer)
{
    texture->renderTexture(getX(), getY(), this->clip);
}
