#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <functional>

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

#endif //CAMERA_HPP