#include <camera.hpp>

void Camera::lookAt(GLfloat x, GLfloat y)
{
    prev_x = this->m_x;
    prev_y = this->m_y;
    this->m_x = x;
    this->m_y = y;
}

GLfloat Camera::getX() const
{
    return m_x;
}

GLfloat Camera::getY() const
{
    return m_y;
}

void Camera::setX(GLfloat x)
{
    prev_x = this->m_x;
     this->m_x = x;
}

void Camera::setY(GLfloat y)
{
    prev_y = this->m_y;
    this->m_y = y;
}

Camera::Camera() : m_x(0.f), m_y(0.f)
{
    prev_x = this->m_x;
    prev_y = this->m_y;
}

void Camera::translate(GLfloat x, GLfloat y)
{
    prev_x = this->m_x;
    prev_y = this->m_y;

    this->m_x += x;
    this->m_y += y;
}

GLfloat Camera::deltaY() const
{
    return m_y - prev_y;
}

GLfloat Camera::deltaX() const
{
    return m_x - prev_x;
}