#include "render/camera.hpp"

void Camera::lookAt(GLfloat x, GLfloat y)
{
    m_prevX = m_x;
    m_prevY = m_y;
    m_x = x;
    m_y = y;
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
    m_prevX = m_x;
    m_x = x;
}

void Camera::setY(GLfloat y)
{
    m_prevY = m_y;
    m_y = y;
}

Camera::Camera() : m_x(0.f), m_y(0.f)
{
    m_prevX = m_x;
    m_prevY = m_y;
}

void Camera::translate(GLfloat x, GLfloat y)
{
    m_prevX = m_x;
    m_prevY = m_y;

    m_x += x;
    m_y += y;
}

GLfloat Camera::deltaY() const
{
    return m_y - m_prevY;
}

GLfloat Camera::deltaX() const
{
    return m_x - m_prevX;
}