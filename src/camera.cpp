#include <camera.hpp>

void Camera::lookAt(GLfloat x, GLfloat y)
{
    this->x = x;
    this->y = y;
}

GLfloat Camera::getX() const
{
    return x;
}

GLfloat Camera::getY() const
{
    return y;
}

void Camera::setX(GLfloat x)
{
     this->x = x;
}

void Camera::setY(GLfloat y)
{
    this->y = y;
}

Camera::Camera() : x(0.f), y(0.f)
{

}

void Camera::translate(GLfloat x, GLfloat y)
{
    this->x += x;
    this->y += y;
}

