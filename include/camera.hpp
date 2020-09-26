#ifndef MOONLANDER_CAMERA_HPP
#define MOONLANDER_CAMERA_HPP

#include <GL/glew.h>

class Camera
{
public:
    Camera();
    void lookAt(GLfloat x, GLfloat y);

    GLfloat getX() const;
    GLfloat getY() const;

    void setX(GLfloat x);
    void setY(GLfloat y);

    GLfloat deltaX() const;
    GLfloat deltaY() const;

    void translate(GLfloat x, GLfloat y);

private:
    GLfloat m_x;
    GLfloat m_y;

    GLfloat m_prevX;
    GLfloat m_prevY;
};

#endif //MOONLANDER_CAMERA_HPP