#include <models/ship.hpp>

Ship::Ship()
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

