#include <models/ship.hpp>

Ship::Ship(ImageTexture *tex, Rect *clip, v_type vel_x, v_type vel_y) :
        Model(tex, clip), vel(vel_x, vel_y), engines_on(false), angle(0.),
        vel_rot(0)
{

}

Ship::~Ship()
{

}

v_type Ship::getVelX() const
{
    return vel.x;
}

v_type Ship::getVelY() const
{
    return vel.y;
}

Vector2D<v_type> Ship::getVel() const
{
    return vel;
}

void Ship::setVelX(v_type v)
{
    vel.x = v;
}

void Ship::setVelY(v_type v)
{
    vel.y = v;
}

void Ship::setVel(Vector2D<v_type> v)
{
    vel.x = v.x;
    vel.y = v.y;
}

void Ship::addVelY(v_type v)
{
    vel.y += v;
}

void Ship::addVelX(v_type v)
{
    vel.x +=v;
}

void Ship::addVel(Vector2D<v_type> v)
{
    vel.x += v.x;
    vel.y += v.y;
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

void Ship::render()
{
    Rect clip{.w = this->clip->w, .h = this->clip->h};

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
    glTranslatef(getX() + clip.w / 2.f, getY() + clip.h / 2.f, 0);
    glRotatef(angle, 0.f, 0.f, 1.f);
    glTranslatef(-(getX() + clip.w / 2.f), -(getY() + clip.h / 2.f), 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    texture->render(getX(), getY(), &clip);

    glPopMatrix();
}

void Ship::rotate(double ang)
{
    angle += ang;
}

double Ship::getAngle() const
{
    return angle;
}

void Ship::addCoords(SDL_Rect coords)
{
    texture->setX(texture->getX() + coords.x);
    texture->setY(texture->getY() + coords.y);
}

void Ship::addY(int y)
{
    texture->setY(texture->getY() + y);
}

void Ship::addX(int x)
{
    texture->setX(texture->getX() + x);
}

v_type Ship::getVelRot() const
{
    return vel_rot;
}

void Ship::setVelRot(v_type vel)
{
    vel_rot = vel;
}

void Ship::addVelRot(v_type vel)
{
    vel_rot += vel;
}
