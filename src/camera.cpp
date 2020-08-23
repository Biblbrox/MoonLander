#include <camera.hpp>

void Camera::lookAt(GLfloat x, GLfloat y)
{
    prev_x = this->x;
    prev_y = this->y;
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
    prev_x = this->x;
     this->x = x;
}

void Camera::setY(GLfloat y)
{
    prev_y = this->y;
    this->y = y;
}

Camera::Camera() : x(0.f), y(0.f)
{
    prev_x = this->x;
    prev_y = this->y;
}

void Camera::translate(GLfloat x, GLfloat y)
{
    prev_x = this->x;
    prev_y = this->y;

    this->x += x;
    this->y += y;
}

GLfloat Camera::deltaY() const
{
    return y - prev_y;
}

GLfloat Camera::deltaX() const
{
    return x - prev_x;
}